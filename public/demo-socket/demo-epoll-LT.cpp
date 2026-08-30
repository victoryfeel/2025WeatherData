#include <iostream>

#include <arpa/inet.h>
#include <cstring>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

int bind_and_listen(int port);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: ./demo-select port" << std::endl;
    return -1;
  }

  int accept_socket = bind_and_listen(atoi(argv[1]));
  if (accept_socket < 0) {
    std::cout << "bind and listen failed\n";
    return -1;
  }
  std::cout << "conn_fd=" << accept_socket << std::endl;

  // 1-epollfd
  int epollfd = epoll_create(1);

  epoll_event event;
  event.data.fd = accept_socket;
  event.events = EPOLLIN | EPOLLET; // set as ET

  // 2-epoll-ctl
  epoll_ctl(epollfd, EPOLL_CTL_ADD, accept_socket, &event);

  epoll_event revent[10];


  // TODO: EVENT LOOP
  while (true) {
    // 3-epoll_wait
    int infds = epoll_wait(epollfd, revent, 10, -1);

    if (infds < 0) {
      perror("poll() failed");
      break;
    }
    if (infds == 0) {
      std::cout << "poll() timeout\n";
      continue;
    }

    for (int i = 0; i < infds; i++) {
      // accept_socket 事件触发 -> client连上
      if (revent[i].data.fd == accept_socket) {
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int conn_fd =
          accept(accept_socket, reinterpret_cast<struct sockaddr*>(&client), &client_len);
        if (conn_fd < 0) {
          perror("accept() failed");
          continue;
        }
        printf("accept client(%d) success.\n", conn_fd);

        // listen the new conn_fd
        event.data.fd = conn_fd;
        event.events = POLLIN;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_fd, &event);
      } else {
        // 其他fd则是有数据读写,或client已经断开
        char buffer[1024];
        memset(&buffer, 0, sizeof(buffer));

        // client断开
        if (recv(revent[i].data.fd, buffer, sizeof(buffer), 0) <= 0) {
          printf("client (%d) disconnected. \n", revent[i].data.fd);
          close(revent[i].data.fd);
          // auto delete closed fd from epoll_fd.
          // epoll_ctl(epollfd, EPOLL_CTL_DEL, revent[i].data.fd, 0);
        } else {
          printf("receive content(%d): %s\n", revent[i].data.fd, buffer);
          send(revent[i].data.fd, buffer, strlen(buffer), 0);
        }
      }
    }
  }

  return 0;
}

int bind_and_listen(int port) {
  int accept_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (accept_socket == -1) {
    perror("socket_fd failed");
    return -1;
  }
  int opt = 1;
  unsigned int opt_len = sizeof(opt);
  setsockopt(accept_socket, SOL_SOCKET, SO_REUSEADDR, &opt, opt_len);

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);

  if (
    bind(
      // accept_socket, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(struct sockaddr_in)
      accept_socket,
      reinterpret_cast<struct sockaddr*>(&server_addr),
      sizeof(server_addr)
    ) == -1
  ) {
    perror("bind() failed");
    close(accept_socket);
    return -1;
  }
  if (listen(accept_socket, 5) != 0) {
    perror("listen failed");
    close(accept_socket);
    return -1;
  }

  return accept_socket;
}
