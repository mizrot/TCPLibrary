#ifndef TCP_STACK
#define TCP_STACK
#include"tcp_types.h"

void push(struct stack_t *s, struct stack_data data);
struct stack_data pop(struct stack_t *s);

#endif
