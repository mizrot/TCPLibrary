#include "api.h"
#include "connect.h"
#include "data.h"
#include "ipv4socket.h"
#include "message.h"
#include "platform.h"
#include "stack.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int tcp_init() { return 0; }

void tcp_create_host(const char *ip, int port, struct tcp_ipv4 *host) {
  host->socket = create_empty_socket();
  host->addr = create_ipv4(ip, port);
  host->is_server = true;
  atomic_init(&(host->should_close), false);
  bind_ipv4_address(host->socket, (struct sockaddr *)&host->addr,
                    sizeof(host->addr));
}

void *_thread_process_socket(void *sock_stack) {
  struct stack_t *stack = ((struct sock_stack_s *)sock_stack)->stack;
  socket_t socket = ((struct sock_stack_s *)sock_stack)->socket;

  char *buff = malloc(sizeof(char) * MAX_MSG_SIZE);

  while (recieve_message(socket, buff) > 0) {
    message_t msg;
    pack_msg(buff, socket, &msg);

    pthread_mutex_lock(&(stack->mutex));
    push(stack, (struct stack_data){.msg = msg});
    pthread_mutex_unlock(&(stack->mutex));
  }
  return NULL;
}

void *_thread_process_connection(void *ptr) {
  size_t thread_count = 2;
  size_t current_thread = 0;

  pthread_t *threads = malloc(sizeof(pthread_t) * thread_count);

  socket_t socket = ((struct socket_addr_stack_s *)ptr)->socket;
  struct sock_stack_s sock_stack =
      ((struct socket_addr_stack_s *)ptr)->sock_stack;
  struct sockaddr_in addr = ((struct socket_addr_stack_s *)ptr)->addr;
  atomic_bool *should_close = ((struct socket_addr_stack_s *)ptr)->should_close;

  while (1) {
    socket_t sock = establish_connection_server(
        socket, (struct sockaddr *)&addr, sizeof(addr));
    if (atomic_load(should_close) || sock == INVALID_SOCKET) {
      break;
    }
    if (current_thread >= thread_count) {
      pthread_t *threads_new = malloc(sizeof(pthread_t) * thread_count * 2);
      memcpy(threads_new, threads, thread_count * sizeof(pthread_t));
      free(threads);
      threads = threads_new;
      thread_count *= 2;
    }
    struct sock_stack_s *args = malloc(sizeof(struct sock_stack_s));
    args->stack = sock_stack.stack;
    args->socket = sock;

    pthread_create(&threads[current_thread], NULL, _thread_process_socket,
                   args);
    current_thread++;
  }
  for (size_t i = 0; i < current_thread + 1; i++) {
    pthread_join(threads[i], NULL);
  }
  free(threads);
  return NULL;
}

void *_thread_process_data(void *ptr) {
  struct stack_t *stack = ((struct stack_callback_s *)ptr)->stack;
  void (*callback)(char *) = ((struct stack_callback_s *)ptr)->ptr;
  atomic_bool *should_close = ((struct stack_callback_s *)ptr)->should_close;
  while (1) {
    if (atomic_load(should_close)) {
      break;
    }
    pthread_mutex_lock(&stack->mutex);
    if (stack->top == NULL) {
      pthread_mutex_unlock(&stack->mutex);
      continue;
    }
    struct stack_data d = pop(stack);
    pthread_mutex_unlock(&(stack->mutex));
    char *buff = malloc(sizeof(char) * (d.msg.len + 1));
    if (unpack_msg(&d.msg, buff, MAX_MSG_SIZE) >= 0) {
      callback(buff);
    }
  }
  return NULL;
}

int tcp_subscribe(struct tcp_ipv4 *server, void (*callback)(char *)) {
  struct stack_t *stack = malloc(sizeof(struct stack_t));
  stack->top = NULL;
  pthread_mutex_init(&stack->mutex, NULL);

  server->process_socket_args =
      (struct sock_stack_s){.stack = stack, .socket = INVALID_SOCKET};
  server->process_connection_args = (struct socket_addr_stack_s){
      .sock_stack = server->process_socket_args,
      .socket = server->socket,
      .addr = server->addr,
      .should_close = &server->should_close,
  };
  server->process_data_args = (struct stack_callback_s){
      .stack = stack, .should_close = &server->should_close, .ptr = callback};

  pthread_create(&server->process_connection, NULL, _thread_process_connection,
                 &server->process_connection_args);
  pthread_create(&server->process_data, NULL, _thread_process_data,
                 &server->process_data_args);

  return 0;
}

void tcp_create_client(const char *ip, int port, struct tcp_ipv4 *host) {
  host->socket = create_empty_socket();
  host->addr = create_ipv4(ip, port);
  host->is_server = false;
}

int tcp_connect(struct tcp_ipv4 *reciever) {
  return establish_connection_client(reciever->socket,
                                     (struct sockaddr *)&(reciever->addr),
                                     sizeof(reciever->addr));
}

int tcp_send(char *msg, struct tcp_ipv4 *reciever) {
  return send_message(reciever->socket, msg, strlen(msg));
}

int tcp_shutdown(struct tcp_ipv4 *target) {
  atomic_store(&(target->should_close), true);
  close_socket(target->socket);
  if (target->is_server) {
    pthread_join(target->process_connection, NULL);
    pthread_join(target->process_data, NULL);
  }
  return 0;
}
