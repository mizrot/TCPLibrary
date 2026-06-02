#include"platform.h"
#include"tcp_threads.h"
#include"queue.h"
#include"thpool.h"
#include<stdlib.h>
#include<time.h>

#define MAX_JOBS 20

void* _thread_job(void *in){
	thpool_t *pool = (thpool_t *)(in);

	lock_mutex(&pool->count_lock);
	pool->num_threads_alive += 1;
	unlock_mutex(&pool->count_lock);

	while(pool->keepalive){

		if (sleep_condition(&pool->jobs.cond) == 0){
			break;
		}

		if (pool->keepalive){

			lock_mutex(&pool->count_lock);
			pool->num_threads_working++;
			unlock_mutex(&pool->count_lock);

			/* Read job from queue and execute it */
			queue_job_t job;

			if (delete_queue(&pool->jobs,(queue_data_t*)&job))
			{
				job.worker(job.arg);
			}

			lock_mutex(&pool->count_lock);
			pool->num_threads_working--;
			if (!pool->num_threads_working) {
				wake_condition(&pool->all_idle);
			}
			unlock_mutex(&pool->count_lock);

		}
	}
	lock_mutex(&pool->count_lock);
	pool->num_threads_alive --;
	unlock_mutex(&pool->count_lock);

	return NULL;

}
thpool_t* thpool_init(int num_threads){
	if (num_threads < 0){
		num_threads = 0;
	}
	thpool_t* pool;
	pool = malloc(sizeof(thpool_t));
	if (pool == NULL){
		print_error("thpool_init(): Could not allocate memory for thread pool\n");
		return NULL;
	}

	pool->num_threads_alive   = 0;
	pool->num_threads_working = 0;
	pool->on_hold   = 0;
	pool->keepalive = 1;

	if (!init_queue(&pool->jobs, MAX_JOBS, QUEUE_JOB)){
		print_error("thpool_init(): Could not allocate memory for job queue ");
		destroy_queue(&(pool->jobs));
		return NULL;
	}
	pool->threads = (os_thread_t**)malloc(num_threads*sizeof(os_thread_t*));
	if (pool->threads == NULL){
		print_error("thpool_init(): Could not allocate memory for threads\n");
		destroy_queue(&pool->jobs);
		free(pool);
		return NULL;
	}
	init_mutex(&(pool->count_lock));
	init_condition(&(pool->all_idle), 0);
	
        for (int n = 0; n<num_threads; ++n){
	    pool->threads[n] = malloc(sizeof(os_thread_t));
	    create_thread(pool->threads[n],_thread_job,pool);
	}	

	return pool;
}

void thpool_add_work(thpool_t *pool, worker_t worker, void *arg){
   queue_job_t newjob;

   newjob.worker = worker;
   newjob.arg = arg;

   insert_queue(&(pool->jobs), (queue_data_t)newjob);
}

void thpool_wait(thpool_t *pool){
   while ( pool->jobs.front == -1 || pool->num_threads_working){
      if (sleep_condition(&(pool->all_idle)) == 0)
	      break;
   }
}

void thpool_destroy(thpool_t *pool){
   if (pool == NULL)
	   return;
   volatile int threads_total = pool->num_threads_alive;

   pool->keepalive = 0;

  double TIMEOUT = 1.0;
  time_t start, end;
  double tpassed = 0.0;
  time(&start);
  while (tpassed < TIMEOUT && pool->num_threads_alive){
	wake_all_condition(&pool->jobs.cond, pool->num_threads_alive);
	os_sleep(1*_TO_SEC);
	time(&end);
	tpassed = difftime(end, start);
  }

  shutdown_condition(&pool->all_idle);

  for (int n = 0; n < threads_total; n++){
	join_thread(pool->threads[n], NULL);
  }

  destroy_queue(&pool->jobs);
  destroy_condition(&pool->all_idle);
  destroy_mutex(&pool->count_lock);
  for (int i = 0; i < pool->num_threads; i++){
	free(pool->threads[i]);
  }
  free(pool->threads);
  free(pool);
}

int thpool_num_threads_working(thpool_t *pool){
  return pool->num_threads_working;
}
