#include <iostream>

#include <arpa/inet.h>
#include <cstring>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
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

  // changed
  pollfd fds[2048];
  for (int i = 0; i < 20148; ++i) {
    fds[i].fd = -1;
  }
  fds[accept_socket].fd = accept_socket;
  fds[accept_socket].events = POLLIN;
  int maxfd = accept_socket;

  while (true) {
    int infds = poll(fds, maxfd + 1, 10000);

    if (infds < 0) {
      perror("poll() failed");
    }
    if (infds == 0) {
      std::cout << "poll() timeout\n";
      continue;
    }

    for (int eventfd = 0; eventfd < maxfd + 1; eventfd++) {
      // changed
      if (fds[eventfd].fd < 0) {
        continue;
      }
      if ((fds[eventfd].revents & POLLIN) == 0) {
        continue;
      }
      // accept_socket 事件触发 -> client连上
      if (eventfd == accept_socket) {
        struct sockaddr_in client;
        unsigned client_len = sizeof(client);
        int conn_fd =
          accept(accept_socket, reinterpret_cast<struct sockaddr*>(&client), &client_len);
        if (conn_fd < 0) {
          perror("accept() failed");
          continue;
        }
        printf("accept client(%d) success.\n", conn_fd);

        // changed
        fds[accept_socket].fd = conn_fd;
        fds[accept_socket].events = POLLIN;
        if (maxfd < conn_fd) {
          maxfd = conn_fd;
        }
      } else {
        // 其他fd则是有数据读写,或client已经断开
        char buffer[1024];
        memset(&buffer, 0, sizeof(buffer));

        // client断开
        if (recv(eventfd, buffer, sizeof(buffer), 0) <= 0) {
          printf("client (%d) disconnected. \n", eventfd);
          close(eventfd);
          // changed
          fds[eventfd].fd = -1;

          if (eventfd == maxfd) {
            for (int i = maxfd; i > 0; --i) {
              // changed
              if (fds[i].fd != -1) {
                maxfd = i;
                break;
              }
            }
          }
        } else {
          // 有数据
          printf("receive content(%d): %s\n", eventfd, buffer);
          // 将数据原封不动发回
          send(eventfd, buffer, strlen(buffer), 0);
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
