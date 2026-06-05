#include"platform.h"
#include"tcp_threads.h"
#include"queue.h"
#include"thpool.h"
#include<stdlib.h>
#include<time.h>

#define MAX_JOBS 1024

void* _thread_job(void *in){
	thpool_t *pool = (thpool_t *)(in);

	lock_mutex(&pool->count_lock);
	pool->num_threads_alive += 1;
	unlock_mutex(&pool->count_lock);

	while(1){

		if (sleep_condition(&pool->jobs.cond) == 0){
			break;
		}

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
	lock_mutex(&pool->count_lock);
	pool->num_threads_alive--;
	unlock_mutex(&pool->count_lock);

	return NULL;

}

int init_thpool(thpool_t *pool, int num_threads){
	if (num_threads < 0){
		num_threads = 0;
	}

	pool->num_threads_alive   = 0;
	pool->num_threads_working = 0;
	pool->on_hold   = 0;
	pool->num_threads = num_threads;

	if (!init_queue(&pool->jobs, MAX_JOBS, QUEUE_JOB)){
		print_error("thpool_init(): Could not allocate memory for job queue ");
		destroy_queue(&(pool->jobs));
		return -1;
	}
	pool->threads = (os_thread_t**)malloc(num_threads*sizeof(os_thread_t*));
	if (pool->threads == NULL){
		print_error("thpool_init(): Could not allocate memory for threads\n");
		destroy_queue(&pool->jobs);
		free(pool);
		return -1;
	}
	init_mutex(&(pool->count_lock));
	init_condition(&(pool->all_idle), 0);
	
        for (int n = 0; n<num_threads; ++n){
	    pool->threads[n] = malloc(sizeof(os_thread_t));
	    create_thread(pool->threads[n],_thread_job,pool);
	}	
	return 0;
}

void add_thpool(thpool_t *pool, worker_t worker, void *arg){
   queue_job_t newjob;

   newjob.worker = worker;
   newjob.arg = arg;

   insert_queue(&(pool->jobs), (queue_data_t)newjob);
}

void wait_thpool(thpool_t *pool){
   while ( pool->jobs.front == -1 || pool->num_threads_working){
      if (sleep_condition(&(pool->all_idle)) == 0)
	      break;
   }
}

void destroy_thpool(thpool_t *pool){
   if (pool == NULL)
	   return;
   volatile int threads_total = pool->num_threads_alive;

  double TIMEOUT = 1.0;
  time_t start, end;
  double tpassed = 0.0;
  time(&start);
  while (tpassed < TIMEOUT && pool->num_threads_alive){
	shutdown_condition(&(pool->jobs.cond));
	shutdown_condition(&(pool->all_idle));
	os_sleep(1*_TO_SEC);
	time(&end);
	tpassed = difftime(end, start);
  }

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
}

int thpool_num_threads_working(thpool_t *pool){
  return pool->num_threads_working;
}
