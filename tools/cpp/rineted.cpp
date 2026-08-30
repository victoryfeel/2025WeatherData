#include "_public.h"
using namespace idc;

clogfile logfile;
cpactive pactive;
void app_exit(int sig);
void app_help();

struct st_route {
  int src_port;
  char dst_ip[31];
  int dst_port;
  int accept_socket;
} stroute;
vector<struct st_route> vroute;
bool load_route_config(const char* inifile);
int bind_and_listen(const int port);

int set_nonblock(int fd);
int epollfd = 0;              // epoll的句柄。
#define MAXSOCK 1024          // 最大连接数。
int conn_fds[MAXSOCK];        // 存放每个socket连接对端的socket的值。
int conn_final_time[MAXSOCK]; // 存放每个socket连接最后一次收发报文的时间。
string conn_buffer[MAXSOCK];  // 存放每个socket发送内容的buffer。

// 向目标地址和端口发起socket连接。
int connect_dst(const char* ip, const int port);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    app_help();
    return -1;
  }
  closeioandsignal(true);
  signal(SIGINT, app_exit);
  signal(SIGTERM, app_exit);
  if (logfile.open(argv[1]) == false) {
    printf("open logfile(%s) failed.\n", argv[1]);
    return -1;
  }
  if (load_route_config(argv[2]) == false) {
    return -1;
  }
  logfile.write("load route-configs-file(%d) success.\n", vroute.size());

  // bind and listen vroute's all port, and set to noblock.
  for (auto& i : vroute) {
    i.accept_socket = bind_and_listen(i.src_port);
    if (i.accept_socket < 0) {
      logfile.write("bind and listen (port-%d) failed.\n", i.src_port);
      continue;
    }
    set_nonblock(i.accept_socket);
  }

  epollfd = epoll_create1(0);
  struct epoll_event event;
  for (auto& i : vroute) {
    event.data.fd = i.accept_socket;
    event.events = POLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, i.accept_socket, &event);
  }

  struct epoll_event revents[10];
  while (true) {
    int read_fds = epoll_wait(epollfd, revents, 10, -1);
    if (read_fds < 0) {
      logfile.write("%s", "epoll_wait() failed.\n");
      app_exit(-1);
    }

    for (int i = 0; i < read_fds; ++i) {
      logfile.write("read event fd=%d (revents:%d).\n", revents[i].data.fd, revents[i].events);
      int j;
      for (j = 0; j < vroute.size(); ++j) {
        if (revents[i].data.fd == vroute[j].accept_socket) {
          // 1-accept_socket, client connect
          struct sockaddr_in client_addr;
          socklen_t len = sizeof(client_addr);
          int conn_fd_src = accept(vroute[j].accept_socket, (struct sockaddr*)&client_addr, &len);
          if (conn_fd_src < 0) {
            break;
          }
          if (conn_fd_src > MAXSOCK) {
            logfile.write("conn_fds more than MAXSOCK: %d.\n", MAXSOCK);
          }
          // 1-1 connect to dst ip and port
          int conn_fd_dst = connect_dst(vroute[j].dst_ip, vroute[j].dst_port);
          if (conn_fd_dst <= 0) {
            close(conn_fd_src);
            break;
          }
          if (conn_fd_dst >= MAXSOCK) {
            logfile.write("sock out of maxsock (%d).\n", MAXSOCK);
            close(conn_fd_dst);
            close(conn_fd_src);
            break;
          }
          logfile.write(
            "connect dst server success. src_port=%d,conn_fd_src=%d,conn_fd_dst=%d.\n",
            vroute[j].src_port,
            conn_fd_src,
            conn_fd_dst
          );

          // add conn_fd_src/dst to epoll
          event.data.fd = conn_fd_src;
          event.events = EPOLLIN;
          epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_fd_src, &event);

          event.data.fd = conn_fd_dst;
          event.events = EPOLLIN;
          epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_fd_dst, &event);
          conn_fds[conn_fd_src] = conn_fd_dst;
          conn_final_time[conn_fd_src] = time(0);
          conn_fds[conn_fd_dst] = conn_fd_src;
          conn_final_time[conn_fd_dst] = time(0);

          break;
        }
      }

      if (j < vroute.size()) {
        continue;
      }
      // 1-channel read
      if (revents[i].events & EPOLLIN) {
        char buffer[4096];
        ssize_t byte_recv = recv(revents[i].data.fd, buffer, sizeof(buffer), 0);
        // disconnected
        if (byte_recv <= 0) {
          logfile.write(
            "client disconnected.(%d:%d).\n", revents[i].data.fd, conn_fds[revents[i].data.fd]
          );
          close(revents[i].data.fd);
          close(conn_fds[revents[i].data.fd]);
          conn_fds[conn_fds[revents[i].data.fd]] = 0;
          conn_fds[revents[i].data.fd] = 0;
          continue;
        }
        // read
        logfile.write(
          "Read: From %d to %d, %d bytes.\n",
          revents[i].data.fd,
          conn_fds[revents[i].data.fd],
          byte_recv
        );
        conn_buffer[conn_fds[revents[i].data.fd]].append(buffer, byte_recv);

        event.data.fd = conn_fds[revents[i].data.fd];
        event.events = EPOLLIN | EPOLLOUT;
        epoll_ctl(epollfd, EPOLL_CTL_MOD, event.data.fd, &event);

        conn_final_time[revents[i].data.fd] = time(0);
        conn_final_time[conn_fds[revents[i].data.fd]] = time(0);
      }
      // 2-channel write
      if (revents[i].events & EPOLLOUT) {
        ssize_t byte_sent = send(
          revents[i].data.fd,
          conn_buffer[revents[i].data.fd].data(),
          conn_buffer[revents[i].data.fd].length(),
          0
        );
        logfile.write("Write: To %d, %d byte.\n", revents[i].data.fd, byte_sent);
        // delete sent data
        conn_buffer[revents[i].data.fd].erase(0, byte_sent);
        // no data, no more delete from epoll wait
        if (conn_buffer[revents[i].data.fd].length() == 0) {
          event.data.fd = revents[i].data.fd;
          event.events = EPOLLIN;
          epoll_ctl(epollfd, EPOLL_CTL_MOD, event.data.fd, &event);
        }
      }
    }
  }

  return 0;
}
// TODO: implement
int connect_dst(const char* ip, const int port) {
  int accept_socket_dst = socket(AF_INET, SOCK_STREAM, 0);
  if (accept_socket_dst < 0) {
    return -1;
  }
  struct hostent* host = gethostbyname(ip);
  if (host == nullptr) {
    close(accept_socket_dst);
    return -1;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  memcpy(&server_addr.sin_addr, host->h_addr, host->h_length);
  server_addr.sin_port = htons(port);

  set_nonblock(accept_socket_dst);

  int isConnected = connect(accept_socket_dst, (sockaddr*)&server_addr, sizeof(server_addr));
  if (isConnected < 0) {
    if (errno != EINPROGRESS) {
      logfile.write("connect dst failed(%s:%d).\n", ip, port);
      return -1;
    }
  }

  return accept_socket_dst;
}

int bind_and_listen(const int port) {
  int accept_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (accept_socket < 0) {
    logfile.write("create accept_socket(%d) failed.\n", port);
    return -1;
  }
  int opt = 1;
  unsigned int len = sizeof(opt);
  setsockopt(accept_socket, SOL_SOCKET, SO_REUSEADDR, &opt, len);

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);

  int isBind = bind(accept_socket, (struct sockaddr*)(&server_addr), sizeof(server_addr));
  if (isBind < 0) {
    logfile.write("bind(%d) error.\n", port);
    close(accept_socket);
    return -1;
  }
  if (listen(accept_socket, SOMAXCONN) < 0) {
    logfile.write("listen(%d) failed.\n", port);
    close(accept_socket);
    return -1;
  }

  return accept_socket;
}

bool load_route_config(const char* inifile) {
  cifile ifile;
  if (ifile.open(inifile) == false) {
    logfile.write("open route config file (%s) failed.\n", inifile);
    return -1;
  }
  string buffer;
  ccmdstr cmdstr;
  while (true) {
    if (ifile.readline(buffer) == false) {
      break;
    }
    int pos = buffer.find('#');
    if (pos != string::npos) {
      buffer.resize(pos);
    }
    replacestr(buffer, "  ", " ", true);
    deletelrchr(buffer, ' ');
    cmdstr.splittocmd(buffer, " ");
    if (cmdstr.size() != 3) {
      continue;
    }

    memset(&stroute, 0, sizeof(st_route));
    cmdstr.getvalue(0, stroute.src_port);
    cmdstr.getvalue(1, stroute.dst_ip);
    cmdstr.getvalue(2, stroute.dst_port);

    vroute.push_back(stroute);
  }


  return true;
}


int set_nonblock(int fd) {
  int flag = fcntl(fd, F_GETFL);
  if (flag == -1) {
    perror("fcntl F_GETFL failed");
    return -1;
  }
  flag = flag | O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flag) == -1) {
    perror("fcntl F_SETFL failed");
    return -1;
  }
  return 0;
}

void app_help() {
  printf("\n");
  printf("Using :./inetd logfile inifile\n\n");
  printf("Sample:./inetd /tmp/inetd.log /etc/inetd.conf\n\n");
  printf(
    "/project/tools/bin/procctl 5 /project/tools/bin/inetd /tmp/inetd.log /etc/inetd.conf\n\n"
  );
  printf("本程序的功能是正向代理，如果用到了1024以下的端口，则必须由root用户启动。\n");
  printf("logfile 本程序运行的日是志文件。\n");
  printf("inifile 路由参数配置文件。\n");
}

void app_exit(int sig) {
  logfile.write("app exit, sig=%d.\n", sig);
  for (auto& i : vroute) {
    if (i.accept_socket > 0) {
      close(i.accept_socket);
    }
  }
  for (auto& i : conn_fds) {
    if (i > 0) {
      close(i);
    }
  }
  close(epollfd);
  exit(0);
}
