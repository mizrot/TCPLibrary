#ifndef IPV4SOCKET_H
#define IPV4SOCKET_H

#include "platform.h"

socket_t create_empty_socket();
struct sockaddr_in create_ipv4(const char *str, int port);

#endif
