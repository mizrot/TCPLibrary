#ifndef TCP_API_H
#define TCP_API_H

#include "tcp_types.h"
#include <stdatomic.h>

int tcp_init();
void tcp_create_host(tcp_ipv4 *, const char *, int);
void tcp_create_client(tcp_ipv4 *, const char *, int);
int tcp_get(const unsigned long int, tcp_ipv4);
int tcp_send(tcp_ipv4 *, char *);
int tcp_receive(tcp_ipv4 *, char *);
int tcp_connect(tcp_ipv4 *);
int tcp_subscribe(tcp_ipv4 *, int, usercallback_t);
int tcp_shutdown(tcp_ipv4 *);
int tcp_destroy();

#endif
