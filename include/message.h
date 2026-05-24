#ifndef TCP_MESSAGE_H
#define TCP_MESSAGE_H

#include<platform.h>
#include<tcp_types.h>

int pack_msg(const char * const msg, tcp_ipv4 dest);
int unpack_msg(message_t msg);

#endif

