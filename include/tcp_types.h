#ifndef TCP_TYPES_H
#define TCP_TYPES_H

#include"platform.h"
#include"tcp_threads.h"
#include<stdatomic.h>
#include<inttypes.h>

#define MAX_MSG_SIZE 1000
typedef struct {
   socket_t socket;
   char *string; 
   unsigned int len;
} queue_message_data_t;

typedef struct {
   char *message;
   size_t msg_len;
   
} queue_buffer_t; 

typedef struct {
   worker_t worker;
   void *arg;
} queue_job_t;

typedef union {
   queue_message_data_t msg;
   queue_buffer_t buff;
   queue_job_t job;
} queue_data_t;

typedef enum {
    QUEUE_MSG,
    QUEUE_BUFFER,
    QUEUE_JOB
} queue_data_type_t;

typedef struct {
   queue_data_t *storage;
   queue_data_type_t type;
   int front;
   int rear;
   int size;
   os_mutex_t mutex;
   os_condition_t cond;
}queue_t;


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

typedef struct node_t{
  node_id_t key;
  socket_t val;
  struct node_t *next;
} node_t;

typedef struct {
 node_t **arr;
 os_mutex_t mutex;
 size_t num, capacity;
}htable_t;

typedef struct {
	socket_t socket;
	struct sockaddr_in addr;
	atomic_bool running;
	unsigned long int id;	
}tcp_ipv4;

#endif

