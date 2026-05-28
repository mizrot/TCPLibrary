#ifndef TCP_TYPES_H
#define TCP_TYPES_H

#include"platform.h"
#include<stdatomic.h>
#include<pthread.h>

#define MAX_MSG_SIZE 1000
typedef struct {
   socket_t socket;
   char *string; 
   unsigned int len;
} message_t;

struct stack_data{
    struct stack_data *next;
    message_t msg;
};

struct stack_t{
  struct stack_data *top;
  pthread_mutex_t mutex;
};

struct sock_stack_s {
  struct stack_t *stack;
  socket_t socket;
};

struct socket_addr_stack_s {
  struct sock_stack_s sock_stack;
  socket_t socket;
  struct sockaddr_in addr;
  atomic_bool *should_close;
};

struct stack_callback_s {
  struct stack_t *stack;
  atomic_bool *should_close;
  void (*ptr)(char *);
};
struct tcp_ipv4{
	socket_t socket;
	struct sockaddr_in addr;
	atomic_bool should_close;
	unsigned long int id;	
	pthread_t process_connection;
	pthread_t process_data;
  struct socket_addr_stack_s process_connection_args;
  struct sock_stack_s process_socket_args;
  struct stack_callback_s process_data_args;
	bool is_server;
};


#endif

