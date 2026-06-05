#include "api.h"
#include "platform.h"

int tcp_init() {

  WSADATA wsa;

  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    return 1;
  }

  return 0;
}

int tcp_destroy() {

  WSACleanup();

  return 0;
}
