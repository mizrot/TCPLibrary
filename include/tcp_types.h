#ifndef TCP_TYPES_H
#define TCP_TYPES_H

#include<platform.h>

struct message_t{
   socket_t socket;
   const char* string; 
};

struct tcp_ipv4{
	socket_t socket;
	struct sockaddr addr;
	unsigned int id;	
};

#endif

