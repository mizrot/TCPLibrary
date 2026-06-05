#include "tcp_threads.h"
#include <inttypes.h>
#include <stdlib.h>

int create_thread(os_thread_t *new_thread, worker_t worker, void *args) {

  const int rc = pthread_create(&(new_thread->handle), NULL, worker, args);
  if (rc != 0) {
    print_error("Couldn't create a thread ");
    return -1;
  }
  return 0;
}

int join_thread(os_thread_t *thread, thread_ret_t *result) {
  void *temp_res;
  const int rc = pthread_join(thread->handle, &temp_res);
  if (rc != 0) {
    print_error("Couldn't join thread");
  }
  if (temp_res != NULL && result != NULL)
    *result = *(thread_ret_t *)(temp_res);
  free(temp_res);
  return 0;
}

void init_mutex(os_mutex_t *mutex) { pthread_mutex_init(mutex, NULL); }
int lock_mutex(os_mutex_t *mutex) { return pthread_mutex_lock(mutex); }
int unlock_mutex(os_mutex_t *mutex) { return pthread_mutex_unlock(mutex); }
void destroy_mutex(os_mutex_t *mutex) { pthread_mutex_destroy(mutex); }

void init_condition(os_condition_t *cond, int value) {
  if (value < 0 || value > 1) {
    print_error("condition can only take values 0 and 1");
    return;
  }
  init_mutex(&cond->mutex);
  pthread_cond_init(&cond->cond, NULL);
  cond->v = value;
}

int sleep_condition(os_condition_t *cond) {
  lock_mutex(&cond->mutex);
  while (cond->v == 0)
    pthread_cond_wait(&cond->cond, &cond->mutex);

  if (cond->v < 0) {
    unlock_mutex(&cond->mutex);
    return 0;
  }
  cond->v--;
  unlock_mutex(&cond->mutex);
  return 1;
}

void wake_condition(os_condition_t *cond) {
  lock_mutex(&(cond->mutex));
  cond->v++;
  pthread_cond_signal(&(cond->cond));
  unlock_mutex(&(cond->mutex));
}

void wake_all_condition(os_condition_t *cond, size_t n) {
  lock_mutex(&(cond->mutex));
  cond->v = n;
  pthread_cond_broadcast(&(cond->cond));
  unlock_mutex(&(cond->mutex));
}

void reset_condition(os_condition_t *cond) {
  pthread_mutex_destroy(&(cond->mutex));
  pthread_cond_destroy(&(cond->cond));
  init_condition(cond, 0);
}

void shutdown_condition(os_condition_t *cond) {
  lock_mutex(&cond->mutex);
  cond->v = -1;
  pthread_cond_broadcast(&(cond->cond));
  unlock_mutex(&cond->mutex);
}

void destroy_condition(os_condition_t *cond) {
  pthread_mutex_destroy(&(cond->mutex));
  pthread_cond_destroy(&(cond->cond));
}
