#include <iostream>

#include <arpa/inet.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("usage: ./demo-client ip port\n");
    return -1;
  }
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    printf("socket() failed\n");
    return -1;
  }

  struct sockaddr_in server_addr;
  char buffer[1024];
  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  if (
    connect(socket_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0
  ) {
    printf("connect(%s:%s) failed", argv[1], argv[2]);
    close(socket_fd);
    return -1;
  }
  printf("connect success!\n");

  // printf("start: %d\n", time(0));
  for (int i = 0; i < 200000; i++) {
    memset(&buffer, 0, sizeof(buffer));
    printf("Please input: ");
    std::cin >> buffer;
    // strcpy(
    //   buffer,
    //   "aaaaaaaaaaaaaaaaaaaaaaaaaaaaalllllllllllllllllllllllllllllllllllleeeeeeeeeeeeeeeeeeeeeeeeeee"
    // );
    if (send(socket_fd, buffer, strlen(buffer), 0) <= 0) {
      printf("send failed");
      close(socket_fd);
      return -1;
    }

    memset(&buffer, 0, sizeof(buffer));
    if (recv(socket_fd, buffer, sizeof(buffer), 0) <= 0) {
      printf("recv() failed\n");
      close(socket_fd);
      return -1;
    }

    printf("recv: %s\n", buffer);
  }

  // printf("end: %d\n", time(0));

  return 0;
}
