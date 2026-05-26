#include <connect.h>
#include <stdio.h>
#include <tcp_types.h>

int establish_connection_client(socket_t clientSocket, struct sockaddr *server,
                                socklen_t len) {

  const unsigned int MAX_TIMEOUT = 8;
  unsigned int timeout = 0;
  while (timeout <= MAX_TIMEOUT && connect(clientSocket, server, len) < 0) {
    print_error("Connect failed. waiting");
    sleep(2);
    timeout += 2;
  }

  if (timeout == MAX_TIMEOUT) {
    print_error("Maximum timeout reached");
    return -1;
  }

  return 1;
}

socket_t establish_connection_server(socket_t serverSocket,
                                     struct sockaddr *client, socklen_t len) {

  while (listen(serverSocket, 5) == SOCKET_ERROR) {
    print_error("Connect was pending too long or connection failed. waiting");
    sleep(2);
  }

  socket_t sock = accept(serverSocket, client, &len);
  if (sock < 0) {
    print_error("Socket busy or not responding. ");
    return INVALID_SOCKET;
  }
  return sock;
}
