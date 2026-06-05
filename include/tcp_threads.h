#ifndef TCP_THREADS_H
#define TCP_THREADS_H



#include"platform.h"



#ifdef PLATFORM_WINDOWS
#include<windows.h>
#endif

#ifdef PLATFORM_LINUX
#include<pthread.h>
#endif




#ifdef PLATFORM_WINDOWS
typedef void* (WINAPI *worker_t)(LPVOID);
#endif

#ifdef PLATFORM_LINUX
typedef void*(*worker_t)(void*) ;
#endif



typedef void* thread_ret_t;

struct thread_data{
 worker_t func;
 void *args;
 thread_ret_t *result;
};

#ifdef PLATFORM_WINDOWS

typedef struct{
   HANDLE handle;
   struct thread_data *data;
}os_thread_t;

#endif

#ifdef PLATFORM_LINUX

typedef struct{
    pthread_t handle;
    struct thread_data *data;
}os_thread_t;

#endif



#ifdef PLATFORM_WINDOWS

typedef CRITICAL_SECTION os_mutex_t;

#endif 

#ifdef PLATFORM_LINUX

typedef pthread_mutex_t os_mutex_t;

#endif


#ifdef PLATFORM_WINDOWS

typedef struct {
	os_mutex_t mutex;
	CONDITION_VARIABLE cond;
	int v;
}os_condition_t;

#endif

#ifdef PLATFORM_LINUX 

typedef struct{
	os_mutex_t mutex;
	pthread_cond_t cond;
	int v;
}os_condition_t;

#endif

int create_thread(os_thread_t *, worker_t, void *);
int join_thread(os_thread_t *, thread_ret_t *);
void init_mutex(os_mutex_t *);
int lock_mutex(os_mutex_t *);
int unlock_mutex(os_mutex_t *);
void destroy_mutex(os_mutex_t *);
void init_condition(os_condition_t *, int);
int sleep_condition(os_condition_t *);
void wake_condition(os_condition_t *);
void wake_all_condition(os_condition_t *, size_t);
void reset_condition(os_condition_t *);
void shutdown_condition(os_condition_t *);
void destroy_condition(os_condition_t *);


#endif 
