#ifndef TCP_RINGBUF_H
#define TCP_RINGBUF_H

#include"tcp_types.h"

int init_ringbuf(ringbuf_t *, int);
void write_ringbuf(ringbuf_t *, char *, int);
uint32_t readable_ringbuf(ringbuf_t *);
void destroy_ringbuf(ringbuf_t *);
void peek_ringbuf(ringbuf_t *, char *, int);
void read_ringbuf(ringbuf_t *, char *, int);
void skip_ringbuf(ringbuf_t *, int);

#endif
