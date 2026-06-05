#include "queue.h"
#include "tcp_threads.h"
#include "tcp_types.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool init_queue(queue_t *queue, int size, queue_data_type_t type) {

  if (size <= 0) {

    queue = NULL;
    return false;

  }

  queue->storage = malloc((size_t)size * sizeof(queue_data_t));

  if (queue->storage == NULL) {

    return false;

  }

  queue->type = type;
  queue->size = size;
  queue->rear = queue->front = -1;
  init_mutex(&queue->mutex);
  init_condition(&queue->cond, 0);

  return true;

}

void destroy_queue(queue_t *queue) {

  free(queue->storage);
  queue->size = 0;
  queue->rear = queue->front = -1;
  destroy_mutex(&(queue->mutex));
  shutdown_condition(&(queue->cond));

}

void insert_queue(queue_t *queue, queue_data_t data) {

  lock_mutex(&queue->mutex);

  if (is_queue_full(queue)) {

    unlock_mutex(&queue->mutex);
    return;

  } else if (queue->rear == -1) {

    queue->rear = queue->front = 0;
    queue->storage[queue->rear] = data;

  } else if (queue->rear >= queue->size - 1) {

    queue->rear = 0;
    queue->storage[queue->rear] = data;

  } else {

    queue->rear += 1;
    queue->storage[queue->rear] = data;

  }

  wake_condition(&(queue->cond));
  unlock_mutex(&(queue->mutex));
}

bool delete_queue(queue_t *queue, queue_data_t *out) {

  lock_mutex(&queue->mutex);

  if (queue->front == -1) {

    return false;

  }

  if (queue->front == queue->rear) {

    int temp = queue->front;
    queue->front = queue->rear = -1;
    *out = queue->storage[temp];

  } else if (queue->front >= queue->size - 1) {

    queue->front = 0;
    *out = queue->storage[queue->size - 1];

  } else {

    queue->front += 1;
    *out = queue->storage[queue->front - 1];
  }

  unlock_mutex(&queue->mutex);
  return true;

}

bool is_queue_full(queue_t *queue) {

  if (queue->front == -1)
    return false;

  return ((queue->rear + 1) % queue->size) == queue->front;

}
