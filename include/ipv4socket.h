#ifndef TCP_IPV4SOCKET_H
#define TCP_IPV4SOCKET_H

#include "platform.h"

socket_t create_empty_socket();
struct sockaddr_in create_ipv4(const char *str, int port);
struct sockaddr_in create_ipv4_server(const char *str, int port);
int bind_ipv4_address(socket_t server, const struct sockaddr_in *addr,
                      socklen_t len);

#endif
