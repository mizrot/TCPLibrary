#ifndef TCP_API_H
#define TCP_API_H

#include"tcp_types.h"
#include <stdatomic.h>

int tcp_init();
void tcp_create_host(const char *, int, struct tcp_ipv4 *);
void tcp_create_client(const char *, int, struct tcp_ipv4 *);
int tcp_get(const unsigned long int, struct tcp_ipv4);
int tcp_send(char *, struct tcp_ipv4 *);
int tcp_connect(struct tcp_ipv4 *);
int tcp_subscribe(struct tcp_ipv4 *, void ( *)(char *));
int tcp_shutdown(struct tcp_ipv4 *);

#endif
