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

int establish_connection_server(socket_t serverSocket,
                                struct sockaddr *client, socklen_t len, socket_t *out) {

  while (listen(serverSocket, 5) == SOCKET_ERROR) {
    print_error("Connect was pending too long or connection failed. waiting");
    sleep(2);
  }

  socket_t sock = accept(serverSocket, (struct sockaddr *)client, &len);
  if (sock == INVALID_SOCKET) {
    print_error("Socket busy or not responding. ");
    return -1;
  }
  *out = sock;
  return 0;
}
