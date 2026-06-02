#ifndef TCP_API_H
#define TCP_API_H

#include"tcp_types.h"
#include <stdatomic.h>

int tcp_init();
void tcp_create_host(const char *, int, tcp_ipv4 *);
void tcp_create_client(const char *, int, tcp_ipv4 *);
int tcp_get(const unsigned long int, tcp_ipv4);
int tcp_send(char *, tcp_ipv4 *);
int tcp_connect(tcp_ipv4 *);
int tcp_subscribe(tcp_ipv4 *, void ( *)(char *));
int tcp_shutdown(tcp_ipv4 *);

#endif
