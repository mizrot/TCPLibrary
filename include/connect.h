#ifndef TCP_CONNECT_H
#define TCP_CONNECT_H

#include <platform.h>
#include <tcp_types.h>

int establish_connection_client(socket_t clientSocket, struct sockaddr *server, socklen_t len);

socket_t establish_connection_server(socket_t serverSocket, struct sockaddr *client, socklen_t len);

#endif
