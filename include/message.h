#ifndef TCP_MESSAGE_H
#define TCP_MESSAGE_H

#include<platform.h>
#include<tcp_types.h>

int pack_msg(const char * const string, socket_t dest, queue_message_data_t *msg );
int unpack_msg(queue_message_data_t *msg, char *buff, size_t buff_size);

#endif

