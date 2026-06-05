#ifndef TCP_THPOOL_H
#define TCP_THPOOL_H
#include"tcp_types.h"

int init_thpool(thpool_t*, int);
void add_thpool(thpool_t*, worker_t, void*);
void wait_thpool(thpool_t*);
void pause_thpool(thpool_t*);
void resume_thpool(thpool_t*);
void destroy_thpool(thpool_t*);
int thpool_num_threads_working(thpool_t*);

#endif
