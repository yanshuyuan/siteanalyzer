#include "thread_pool.h"

void thread_pool_init(thread_pool_t *thread_pool, size_t thread_count)
{
/*
    printf("---------thread_pool_init malloc size: %d================\n", sizeof(thread_t) * thread_count);
*/
    if((thread_pool->thread_items = (thread_t *) malloc(sizeof(thread_t) * thread_count)) == NULL) {
	fprintf(stderr, "Error: allocate memory failed in func 'thread_pool_init'.\n");	
    } else {
	size_t i;
	for(i = 0; i < thread_count; i++) {
	    thread_init(&thread_pool->thread_items[i]);
        }
	thread_pool->thread_count = thread_count;
	ts_queue_init(&thread_pool->job_queue);
	sem_init(&thread_pool->job_sem, 0, 0);
	thread_counter_init(&thread_pool->job_done);
	thread_counter_init(&thread_pool->thread_idle);
	thread_counter_set(&thread_pool->thread_idle, thread_count);
    }
}

void thread_pool_destroy(thread_pool_t *thread_pool)
{
    size_t i;
    for(i = 0; i < thread_pool->thread_count; i++) {
	thread_destroy(&thread_pool->thread_items[i]);
    }
    free(thread_pool->thread_items);
    ts_queue_destroy(&thread_pool->job_queue);
    sem_destroy(&thread_pool->job_sem);
    thread_counter_destroy(&thread_pool->job_done);
    thread_counter_destroy(&thread_pool->thread_idle);
}

void thread_pool_join(thread_pool_t *thread_pool)
{
    size_t i;
    for(i = 0; i < thread_pool->thread_count; i++) {
    	thread_join(&thread_pool->thread_items[i]);
    }

}

void thread_pool_wait_job_semaphare(thread_pool_t *thread_pool)
{
    sem_wait(&thread_pool->job_sem);
}

void thread_pool_post_job_semaphare(thread_pool_t *thread_pool)
{
    sem_post(&thread_pool->job_sem);
}

void thread_pool_get_job(thread_pool_t *thread_pool, job_t *job)
{
    ts_queue_top(&thread_pool->job_queue, job);
}

void thread_pool_set_job(thread_pool_t *thread_pool, job_t *job)
{
    ts_queue_push(&thread_pool->job_queue, job, sizeof(job_t));
}

void *thread_do_job(void *arg)
{
    thread_info_t *thread_info = (thread_info_t *) arg;
    job_t job;
    thread_set_idle(thread_info->thread_item, THREAD_BUSY);
    while(thread_get_idle(thread_info->thread_item) == THREAD_BUSY) {
	/* wait job semaphre */
	thread_pool_wait_job_semaphare(thread_info->thread_pool);
	/* update thread in thread_pool status */
	thread_counter_decrease(&thread_info->thread_pool->thread_idle);
	/* get a job and do it */
        thread_pool_get_job(thread_info->thread_pool, &job);
	job_arg_t job_arg;
	job_arg.thread_item = thread_info->thread_item;
	job_arg.arg = job.arg; 
	printf("threadID: %d start work\n", thread_info->thread_item->id);
	job.run(&job_arg);
	printf("threadID: %d work end\n", thread_info->thread_item->id);
	/* update thread in thread_pool status */
	thread_counter_increase(&thread_info->thread_pool->thread_idle);
	thread_counter_increase(&thread_info->thread_pool->job_done);
	thread_counter_increase(&thread_info->thread_item->job_done);
    }
	/*
    printf("=========threadID: %d run over========================\n", thread_info->thread_item->id);
	*/
    free(thread_info);
    return 0;
}

void thread_pool_run(thread_pool_t *thread_pool)
{
    size_t i;
    thread_info_t *thread_info;
    for(i = 0; i < thread_pool->thread_count; i++) {
	/* need free in thread_do_job */
	thread_info = (thread_info_t *)malloc(sizeof(thread_info_t));

	thread_info->thread_item = &thread_pool->thread_items[i];
	thread_info->thread_item->id = i;

	thread_info->thread_pool = thread_pool;

	int ret = thread_create(&thread_pool->thread_items[i], NULL, thread_do_job, thread_info);
	if(ret != 0) {
	    fprintf(stderr, "thread_pool's thread %d create failed, retval: %d.\n", i, ret);
	}
    }
}

