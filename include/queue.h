#ifndef TCP_QUEUE_H
#define TCP_QUEUE_H
#include "tcp_types.h"
#include <stdbool.h>

bool init_queue(queue_t *, int, queue_data_type_t);
void insert_queue(queue_t *, queue_data_t);
bool delete_queue(queue_t *, queue_data_t *);
void destroy_queue(queue_t *);
bool is_queue_full(queue_t *);
#endif
