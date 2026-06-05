#ifndef TCP_MESSAGE_H
#define TCP_MESSAGE_H

#include<platform.h>
#include<tcp_types.h>

int pack_msg(const char * const , socket_t , queue_message_data_t *);
int unpack_msg(queue_message_data_t *, char *, size_t);

#endif

