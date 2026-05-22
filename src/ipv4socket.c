#include "../include/ipv4socket.h"

socket_t create_empty_socket() {
  socket_t sock;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  return sock;
}

struct sockaddr_in create_ipv4(const char *str, int port) {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, str, &addr.sin_addr);
  return addr;
}
