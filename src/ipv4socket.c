#include "../include/ipv4socket.h"
#include <stdio.h>
#include <string.h>
socket_t create_empty_socket() {
  socket_t sock;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == INVALID_SOCKET) {
    perror("Couldn't create a new socket.");
    return sock;
  }
  return sock;
}

struct sockaddr_in create_ipv4(const char *str, int port) {
  struct sockaddr_in addr;
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  if (inet_pton(AF_INET, str, &addr.sin_addr) <= 0) {
    perror("Invalid address or address not supported.\n");
    return (struct sockaddr_in){0};
  }
  return addr;
}

void close_socket(socket_t socket) { close(socket); }
