#include "api.h"
#include "connect.h"
#include "data.h"
#include "htable.h"
#include "ipv4socket.h"
#include "platform.h"
#include "queue.h"
#include "ringbuf.h"
#include "thpool.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void tcp_create_host(tcp_ipv4 *host, const char *ip, int port) {

  host->socket = create_empty_socket();
  host->addr = create_ipv4_server(ip, port);
  bind_ipv4_address(host->socket, (&host->addr), sizeof(host->addr));
  init_htable((&host->clients));
  init_queue((&host->messages), 100, QUEUE_MSG);
}

void *_thread_process_socket(void *ptr) {

  client_data_t client = ((node_t *)ptr)->val;

  tcp_ipv4 *host = client.host;

  char tmp[MAX_RECV_SIZE];

  while (atomic_load(&host->running)) {

    int n = receive_message(client.socket, tmp);
    if (n <= 0)
      break;

    write_ringbuf(client.recv_buf, tmp, n);

    while (readable_ringbuf(client.recv_buf) >= 4) {

      uint32_t len;
      peek_ringbuf(client.recv_buf, (char *)(&len), 4);
      len = ntohl(len);

      if (readable_ringbuf(client.recv_buf) < 4 + len)
        break;

      skip_ringbuf(client.recv_buf, 4);
      queue_message_data_t msg;

      msg.string = malloc(sizeof(char) * len + 1);
      msg.string[len] = '\0';
      msg.socket = client.socket;
      msg.len = len;

      read_ringbuf(client.recv_buf, msg.string, len);
      insert_queue(&(host->messages), ((queue_data_t)msg));
    }
  }
  delete_htable(&(host->clients), client.addr.sin_addr.s_addr);
  return NULL;
}

void *_thread_process_connection(void *ptr) {

  tcp_ipv4 *host = ((struct process_connection_ctx *)ptr)->host;
  int threads = ((struct process_connection_ctx *)ptr)->threads;

  thpool_t pool;
  init_thpool(&pool, threads);

  while (atomic_load(&(host->running))) {

    struct sockaddr_in new_client_addr;
    socket_t client_sock = establish_connection_server(
        host->socket, &new_client_addr, sizeof(new_client_addr));

    if (client_sock == INVALID_SOCKET) {
      continue;
    }

    ringbuf_t *rb = malloc(sizeof(ringbuf_t));
    init_ringbuf(rb, RECV_RINGBUF_SIZE);

    client_data_t client = {.socket = client_sock, .recv_buf = rb, .addr = new_client_addr, .host = host};
    insert_htable(&(host->clients), new_client_addr.sin_addr.s_addr, client);

    add_thpool(
        &pool, _thread_process_socket,
        search_htable(&(host->clients), new_client_addr.sin_addr.s_addr));
  }

  destroy_htable(&host->clients);
  destroy_thpool(&pool);

  return NULL;
}

void *_thread_process_data(void *ptr) {

  void (*func)(tcp_event_ctx_t *) = ((struct process_data_ctx *)ptr)->func;

  tcp_ipv4 *host = ((struct process_data_ctx *)ptr)->host;

  while (atomic_load(&(host->running))) {

    sleep_condition(&(host->messages.cond));

    queue_message_data_t msg;
    if (!delete_queue(&(host->messages), (queue_data_t *)&msg)) {
      continue;
    }

    tcp_event_ctx_t ctx = {.clients = &host->clients,
                           .host = host->socket,
                           .sender = msg.socket,
                           .msg = msg.string};

    func(&ctx);
    free(msg.string);
  }

  destroy_queue(&host->messages);

  return NULL;
}

int tcp_subscribe(tcp_ipv4 *host, int threads, usercallback_t callback) {

  host->running = true;
  host->th_con = malloc(sizeof(process_connection_t));

  if (host->th_con == NULL) {

    print_error("Couldn't allocate memory for thread ");
    return -1;
  }

  host->th_data = malloc(sizeof(process_data_t));

  if (host->th_data == NULL) {

    print_error("Couldn't allocate memory for thread ");
    return -1;
  }

  host->th_con->ctx =
      (struct process_connection_ctx){.host = host, .threads = threads};
  create_thread((&host->th_con->thread), _thread_process_connection,
                &(host->th_con->ctx));

  host->th_data->ctx =
      (struct process_data_ctx){.host = host, .func = callback};
  create_thread((&host->th_data->thread), _thread_process_data,
                &(host->th_data->ctx));

  return 0;
}

void tcp_create_client(tcp_ipv4 *host, const char *ip, int port) {

  host->socket = create_empty_socket();
  host->addr = create_ipv4_server(ip, port);
  host->running = false;
}

int tcp_connect(tcp_ipv4 *reciever) {

  if (establish_connection_client(reciever->socket, (&reciever->addr),
                                  sizeof(reciever->addr))) {
    return -1;
  }

  return 0;
}

int tcp_send(tcp_ipv4 *reciever, char *msg) {

  if (send_message(reciever->socket, msg)) {

    return -1;
  }

  return 0;
}

int tcp_shutdown(tcp_ipv4 *target) {

  if (atomic_load(&target->running) == true) {

    atomic_store((&target->running), false);
    shutdown(target->socket, SHUTDOWN);
    close_socket(target->socket);
    wake_condition(&target->messages.cond);

    join_thread(&(target->th_con->thread), NULL);
    join_thread(&(target->th_data->thread), NULL);

    free(target->th_con);
    free(target->th_data);

  } else {

    shutdown(target->socket, SHUTDOWN);
    close_socket(target->socket);
  }

  return 0;
}
