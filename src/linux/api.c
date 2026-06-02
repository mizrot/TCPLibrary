#include "api.h"
#include "connect.h"
#include "data.h"
#include "ipv4socket.h"
#include "message.h"
#include "platform.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int tcp_init() { return 0; }

void tcp_create_host(const char *ip, int port, tcp_ipv4 *host) {
  host->socket = create_empty_socket();
  host->addr = create_ipv4_server(ip, port);
}

void *_thread_process_socket(void *sock_stack) {}

void *_thread_process_connection(void *ptr) {}

void *_thread_process_data(void *ptr) {}

int tcp_subscribe(tcp_ipv4 *server, void (*callback)(char *)) {}

void tcp_create_client(const char *ip, int port, tcp_ipv4 *host) {}

int tcp_connect(tcp_ipv4 *reciever) {}

int tcp_send(char *msg, tcp_ipv4 *reciever) {}

int tcp_shutdown(tcp_ipv4 *target) {}
