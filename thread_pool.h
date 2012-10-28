#ifndef _THREAD_POOL
#define _THREAD_POOL

#include <pthread.h>
#include <semaphore.h>
#include "ts_queue.h"
#include "thread.h"
#include "thread_counter.h"
#include "job.h"

#define THREAD_IDLE 0
#define THREAD_BUSY 1

typedef struct _thread_pool {
    thread_t *thread_items;
    size_t thread_count;
    sem_t job_sem;
    thread_counter_t job_done;
    thread_counter_t thread_idle;
    ts_queue_t job_queue;
} thread_pool_t;

typedef struct _thread_info {
    thread_pool_t *thread_pool;
    thread_t *thread_item;
} thread_info_t;


void thread_pool_init(thread_pool_t *thread_pool, size_t thread_count);
void thread_pool_destroy(thread_pool_t *thread_pool);
void thread_pool_run(thread_pool_t *tread_pool);
void thread_pool_join(thread_pool_t *thread_pool);
void thread_pool_post_job_semaphare(thread_pool_t *thread_pool);
void thread_pool_wait_job_semaphare(thread_pool_t *thread_pool);
void thread_pool_get_job(thread_pool_t *thread_pool, job_t *job);
void thread_pool_set_job(thread_pool_t *thread_pool, job_t *job);

#endif


