#include "connect.h"

int establish_connection_client(socket_t clientSocket,
                                struct sockaddr_in *server, socklen_t len) {

  const unsigned int MAX_TIMEOUT = 8;
  unsigned int timeout = 0;

  while (timeout <= MAX_TIMEOUT &&
         connect(clientSocket, (struct sockaddr *)server, len) ==
             SOCKET_ERROR) {

    print_error("Connect failed. waiting");
    os_sleep(2 * _TO_SEC);
    timeout += 2 * _TO_SEC;

  }

  if (timeout == MAX_TIMEOUT) {

    print_error("Maximum timeout reached");
    return -1;

  }

  return 0;
}

socket_t establish_connection_server(socket_t serverSocket,
                                     struct sockaddr_in *client,
                                     socklen_t len) {

  socket_t sock = accept(serverSocket, (struct sockaddr *)client, &len);

  if (sock == INVALID_SOCKET) {

    print_error("Socket busy or not responding");
    return INVALID_SOCKET;

  }

  return sock;

}
