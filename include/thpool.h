#ifndef TCP_THPOOL_H
#define TCP_THPOOL_H
#include"tcp_types.h"

thpool_t* thpool_init(int);
void thpool_add_work(thpool_t*, worker_t, void*);
void thpool_wait(thpool_t*);
void thpool_pause(thpool_t*);
void thpool_resume(thpool_t*);
void thpool_destroy(thpool_t*);
int thpool_num_threads_working(thpool_t*);

#endif
