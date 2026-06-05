#ifndef TCP_CONNECT_H
#define TCP_CONNECT_H

#include <platform.h>
#include <tcp_types.h>

int establish_connection_client(socket_t, struct sockaddr_in*, socklen_t);

socket_t establish_connection_server(socket_t , struct sockaddr_in*, socklen_t );

#endif
