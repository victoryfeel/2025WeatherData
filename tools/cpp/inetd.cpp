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
int conntodst(const char* ip, const int port);


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


  return 0;
}
// TODO: implement

bool load_route_config(const char* inifile) {
  cifile ifile;
  if (ifile.open(inifile) == false) {
    logfile.write("open route config file (%s) failed.\n", inifile);
    return -1;
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
