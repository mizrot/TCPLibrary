#ifndef TCP_HTABLE_H
#define TCP_HTABLE_H
#include"tcp_types.h"

int init_htable(htable_t *);
void insert_htable(htable_t *, node_id_t, socket_t);
void delete_htable(htable_t *, node_id_t);
socket_t search_htable(htable_t *, node_id_t);

#endif
