#include"platform.h"
#include"tcp_types.h"
#include"stack.h"
#include<stdlib.h>

void push(struct stack_t *s, struct stack_data data){
	if (s->top == NULL){
		s->top = malloc(sizeof(struct stack_data));
		*(s->top) = data;
		s->top->next = NULL;
		return;
	}
	struct stack_data *node = s->top;
	node->next = s->top;
	s->top = node;
}

struct stack_data pop(struct stack_t *s){
	if (s->top == NULL){
	return (struct stack_data){};
	}
	struct stack_data *old = s->top;

	struct stack_data temp = *old;

	s->top = s->top->next;

	free(old);

	return temp;
}

