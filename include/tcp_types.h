#ifndef TCP_TYPES_H
#define TCP_TYPES_H

#include "platform.h"
#include "tcp_threads.h"
#include <inttypes.h>
#include <stdatomic.h>

#define MAX_RECV_SIZE 1024
#define RECV_RINGBUF_SIZE ((MAX_RECV_SIZE) * 10)

typedef struct {
  socket_t socket;
  char *string;
  uint32_t len;
} queue_message_data_t;

typedef struct {
  worker_t worker;
  void *arg;
} queue_job_t;

typedef union {
  queue_message_data_t msg;
  queue_job_t job;
} queue_data_t;

typedef enum { QUEUE_MSG, QUEUE_JOB } queue_data_type_t;

typedef struct {
  queue_data_t *storage;
  queue_data_type_t type;
  int front;
  int rear;
  int size;
  os_mutex_t mutex;
  os_condition_t cond;
} queue_t;

typedef struct {
  char *storage;
  int front;
  int rear;
  int size;
  int count;
} ringbuf_t;

typedef struct {
  os_thread_t **threads;
  queue_t jobs;
  volatile int num_threads_alive;
  volatile int num_threads_working;
  volatile int on_hold;
  volatile int num_threads;
  os_mutex_t count_lock;
  os_condition_t all_idle;
} thpool_t;

typedef uint32_t node_id_t;

typedef struct tcp_ipv4 tcp_ipv4;

typedef struct {
  socket_t socket;
  ringbuf_t *recv_buf;
  tcp_ipv4 *host;
} client_data_t;

typedef struct node_t {
  node_id_t key;
  client_data_t val;
  struct node_t *next;
} node_t;

typedef struct {
  node_t **arr;
  os_mutex_t mutex;
  size_t num, capacity;
} htable_t;

typedef struct {
  htable_t *clients;
  socket_t sender;
  tcp_ipv4* host;
  char *msg;
} tcp_event_ctx_t;

typedef void (*usercallback_t)(tcp_event_ctx_t *);

struct process_data_ctx {
  usercallback_t func;
  tcp_ipv4 *host;
};

struct process_connection_ctx {
  tcp_ipv4 *host;
  int threads;
};
typedef struct {
  struct process_connection_ctx ctx;
  os_thread_t thread;
} process_connection_t;

typedef struct {
  struct process_data_ctx ctx;
  os_thread_t thread;
} process_data_t;

typedef struct tcp_ipv4 {
  socket_t socket;
  struct sockaddr_in addr;
  htable_t clients;
  thpool_t pool;
  queue_t messages;
  process_connection_t *th_con;
  process_data_t *th_data;

  atomic_bool running;
} tcp_ipv4;

#endif
