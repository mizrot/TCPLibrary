#ifndef TCP_DATA_H
#define TCP_DATA_H

#include<platform.h>
#include<tcp_types.h>

int recieve_message(socket_t socket, char *msg);
int send_message(socket_t socket, const char *msg, unsigned int len);

#endif
