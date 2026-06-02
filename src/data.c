#include <data.h>
#include <ipv4socket.h>
#include <platform.h>
#include <stdlib.h>
#include <tcp_types.h>

int recieve_message(socket_t socket, char *msg) {

  int code = recv(socket, msg, MAX_MSG_SIZE, 0);
  if (code != SOCKET_ERROR) {

    if (code == 0) {

      close_socket(socket);
      return code;
    }

    goto success;

  } else {

    print_error("Coudn't recieve a message");
    return code;
  }
success:
  return code;
}

int send_message(socket_t socket, const char *msg, unsigned int len) {

  if (send(socket, msg, len, 0) < 0) {

    print_error("Coudn't send a message");
    return -1;
  }

  return 0;
}
