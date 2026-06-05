#include "ringbuf.h"

#include <stdlib.h>
#include <string.h>

int init_ringbuf(ringbuf_t *ringbuf, int size) {

  if (size <= 0) {
    return -1;
  }

  ringbuf->storage = malloc(sizeof(char) * size);

  if (ringbuf->storage == NULL) {

    print_error("Couldn't allocate memory for ringbuffer ");
    return -1;
  }

  ringbuf->front = 0;
  ringbuf->rear = 0;
  ringbuf->size = size;
  ringbuf->count = 0;

  return 0;
}

void write_ringbuf(ringbuf_t *ringbuf, char *data, int len) {

  if (ringbuf->count + len > ringbuf->size) {
    return;
  }

  int first_idx = (ringbuf->rear) % ringbuf->size;

  if (first_idx + len <= ringbuf->size) {
    memcpy(ringbuf->storage + first_idx, data, len);

  } else {

    int first_part = ringbuf->size - first_idx;
    int second_part = len - first_part;

    memcpy(ringbuf->storage + first_idx, data, first_part);
    memcpy(ringbuf->storage, data + first_part, second_part);
  }

  ringbuf->rear = (ringbuf->rear + len) % ringbuf->size;
  ringbuf->count += len;
}

void read_ringbuf(ringbuf_t *ringbuf, char *data, int len) {

  if (ringbuf->count < len) {
    return;
  }

  int first_idx = ringbuf->front;

  if (first_idx + len <= ringbuf->size) {

    memcpy(data, ringbuf->storage + first_idx, len);

  } else {

    int first_part = ringbuf->size - first_idx;
    int second_part = len - first_part;
    memcpy(data, ringbuf->storage + first_idx, first_part);
    memcpy(data + first_part, ringbuf->storage, second_part);
  }
  ringbuf->front = (ringbuf->front + len) % ringbuf->size;
  ringbuf->count -= len;

  if (ringbuf->count == 0) {

    ringbuf->front = 0;
    ringbuf->rear = 0;
  }
}

void peek_ringbuf(ringbuf_t *ringbuf, char *data, int len) {

  if (ringbuf->count < len) {
    return;
  }

  int first_idx = ringbuf->front;

  if (first_idx + len <= ringbuf->size) {

    memcpy(data, ringbuf->storage + first_idx, len);

  } else {

    int first_part = ringbuf->size - first_idx;
    int second_part = len - first_part;
    memcpy(data, ringbuf->storage + first_idx, first_part);
    memcpy(data + first_part, ringbuf->storage, second_part);
  }
}

void skip_ringbuf(ringbuf_t *ringbuf, int len) {

  if (ringbuf->count < len) {
    return;
  }

  ringbuf->front = (ringbuf->front + len) % ringbuf->size;
  ringbuf->count -= len;
}

uint32_t readable_ringbuf(ringbuf_t *ringbuf) {

  return (ringbuf->rear - ringbuf->front + ringbuf->size) % ringbuf->size;
}

void destroy_ringbuf(ringbuf_t *ringbuf) { free(ringbuf->storage); }

