#ifndef CONNECT_H
#define CONNECT_H

#include "platform.h"

int establish_connection_client(socket_t socket, struct sockaddr* server);
int establish_connection_server(socket_t socket, struct sockaddr* client);

#endif
