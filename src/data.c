#include <data.h>
#include <ipv4socket.h>
#include <platform.h>
#include <string.h>
#include <stdlib.h>
#include <tcp_types.h>
#include <inttypes.h>

int receive_message(socket_t socket, char *msg) {
  int code = recv(socket, msg, MAX_RECV_SIZE, 0);
  if (code != SOCKET_ERROR) {
    if (code == 0) {
      close_socket(socket);
      return code;
    }
  } else {
    print_error("Couldn't receive a message");
    return code;
  }
  return code;
}

int send_message(socket_t socket, const char *msg) {
  uint32_t len = strlen(msg);
  uint32_t net_len = htonl(len);
  char *packet = malloc(sizeof(uint32_t) + len);

  memcpy(packet, &net_len, sizeof(net_len));
  memcpy(packet + sizeof(net_len), msg, len);
  if (send(socket, packet, sizeof(uint32_t)+len, 0) < 0) {
    print_error("Couldn't send a message");
    return -1;
  }
  free(packet);
  return 0;
}
