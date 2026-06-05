#include "tcp_threads.h"

DWORD WINAPI _win_wrapper(LPVOID args) {

  struct thread_data *data = (struct thread_data *)args;

  data->result = data->func(data->args);

  return 0;
}

int create_thread(os_thread_t *new_thread, worker_t worker, void *args) {

  struct thread_data *data = malloc(sizeof(struct thread_data));

  data->func = worker;
  data->args = args;

  new_thread->data = data;

  new_thread->handle = CreateThread(NULL, 0, _win_wrapper, data, 0, NULL);

  if (new_thread->handle == NULL) {

    free(data);
    print_error("Coudn't create a thread");

    return -1;
  }

  return 0;
}

int join_thread(os_thread_t *thread, thread_ret_t *result) {

  WaitForSingleObject(thread->handle, INFINITE);
  CloseHandle(thread->handle);

  if (result != NULL && thread->data->result != NULL)
    *result = *(thread_ret_t *)(thread->data->result);

  free(thread->data);

  return 0;
}

void init_mutex(os_mutex_t *mutex) { InitializeCriticalSection(mutex); }

int lock_mutex(os_mutex_t *mutex) {

  EnterCriticalSection(mutex);
  return 0;

}

int unlock_mutex(os_mutex_t *mutex) {
	
  LeaveCriticalSection(mutex);
  return 0;

}

void destroy_mutex(os_mutex_t *mutex) { DeleteCriticalSection(mutex); }

void init_condition(os_condition_t *cond, int value) {

  init_mutex(&cond->mutex);
  InitializeConditionVariable(&cond->cond);
  cond->v = value;

}

int sleep_condition(os_condition_t *cond) {

  lock_mutex(&cond->mutex);

  while (cond->v == 0)
    SleepConditionVariableCS(&cond->cond, &cond->mutex, INFINITE);

  if (cond->v < 0) {
    unlock_mutex(&cond->mutex);
    return 0;

  }

  cond->v = 0;
  unlock_mutex(&cond->mutex);

  return 1;
}

void wake_condition(os_condition_t *cond) {

  lock_mutex(&cond->mutex);
  cond->v++;
  WakeConditionVariable(&(cond->cond));
  unlock_mutex(&cond->mutex);

}

void wake_all_condition(os_condition_t *cond, size_t n) {

  lock_mutex(&cond->mutex);
  cond->v = n;
  WakeAllConditionVariable(&(cond->cond));
  unlock_mutex(&cond->mutex);

}

void reset_condition(os_condition_t *cond) {

  DeleteCriticalSection(&(cond->mutex));
  init_condition(cond, 0);

}

void shutdown_condition(os_condition_t *cond) {

  lock_mutex(&cond->mutex);
  cond->v = -1;
  WakeAllConditionVariable(&(cond->cond));
  unlock_mutex(&cond->mutex);

}


void destroy_condition(os_condition_t *cond) {

  DeleteCriticalSection(&(cond->mutex));

}
