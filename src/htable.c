#include "htable.h"
#include "platform.h"
#include "ringbuf.h"
#include "tcp_types.h"
#include <stdlib.h>
#include <string.h>

int init_htable(htable_t *mp) {

  mp->capacity = 100;
  mp->num = 0;
  init_mutex(&mp->mutex);

  mp->arr = malloc(sizeof(node_t *) * mp->capacity);

  if (mp->arr == NULL) {

    print_error("init_htable failed to initialize");
    return -1;
  }

  for (size_t i = 0; i < mp->capacity; i++){
	mp->arr[i] = NULL;
  }
  return 0;
}

int hashFunction(htable_t *mp, node_id_t id) {

  char key[10];

  sprintf(key, "%u", id);

  int bucketIndex;
  int sum = 0, factor = 31;

  for (size_t i = 0; i < strlen(key); i++) {

    sum = ((sum % mp->capacity) + (((int)key[i]) * factor) % mp->capacity) %
          mp->capacity;

    factor = ((factor % __INT16_MAX__) * (31 % __INT16_MAX__)) % __INT16_MAX__;
  }

  bucketIndex = sum;
  return bucketIndex;
}

void insert_htable(htable_t *mp, node_id_t id, client_data_t data) {

  lock_mutex(&(mp->mutex));
  int bucketIndex = hashFunction(mp, id);

  node_t *peer = malloc(sizeof(node_t));
  peer->key = id;
  peer->val = data;
  peer->next = NULL;

  if (mp->arr[bucketIndex] == NULL) {

    mp->arr[bucketIndex] = peer;

  } else {

    peer->next = mp->arr[bucketIndex];
    mp->arr[bucketIndex] = peer;
  }
  unlock_mutex(&(mp->mutex));
}

void delete_htable(htable_t *mp, node_id_t id) {

  lock_mutex(&(mp->mutex));
  int bucketIndex = hashFunction(mp, id);
  node_t *curr = mp->arr[bucketIndex];
  node_t *prev;

  while (curr != NULL) {

    if (id == curr->key) {

      if (curr == mp->arr[bucketIndex]) {
        mp->arr[bucketIndex] = curr->next;

      } else {

        prev->next = curr->next;
      }

      free(curr);
      break;
    }

    prev = curr;
    curr = curr->next;
  }

  unlock_mutex(&(mp->mutex));
}

void drain_htable(htable_t *mp, void (*callback)(node_t *)) {
   lock_mutex(&(mp->mutex));

  for (size_t i = 0; i < mp->capacity; i++) {

    node_t *cur = mp->arr[i];

    while (cur != NULL) {

      node_t *next = cur->next;
      callback(cur);
      cur = next;
    }

    mp->arr[i] = NULL;
  }
  unlock_mutex(&(mp->mutex));
}

node_t *search_htable(htable_t *mp, node_id_t id) {

  int bucketIndex = hashFunction(mp, id);
  node_t *curr = mp->arr[bucketIndex];

  while (curr != NULL) {

    if (curr->key == id) {
      return curr;
    }

    curr = curr->next;
  }

  return NULL;
}

  void destroy_connection(node_t * node) {
    close_socket(node->val.sock);
    destroy_ringbuf(node->val.recv_buf);
    free(node->val.recv_buf);
    free(node);
  }

void destroy_htable(htable_t *mp){
   drain_htable(mp, destroy_connection);
   free(mp->arr);
}
