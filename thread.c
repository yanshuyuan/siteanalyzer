#include "thread.h"

void thread_init(thread_t *thread_item)
{
    thread_item->threadinfo = 0;
    thread_item->id = -1;
    thread_item->idle = 0;
    thread_counter_init(&thread_item->job_done);
    pthread_mutex_init(&thread_item->waitlock, NULL);
}

void thread_destroy(thread_t *thread_item)
{
    thread_counter_destroy(&thread_item->job_done);
    pthread_mutex_destroy(&thread_item->waitlock);
}

void thread_set_idle(thread_t *thread_item, int idle)
{
    pthread_mutex_lock(&thread_item->waitlock);
    thread_item->idle = idle;
    pthread_mutex_unlock(&thread_item->waitlock);
}

int thread_get_idle(thread_t *thread_item)
{
    int idle;
    pthread_mutex_lock(&thread_item->waitlock);
    idle = thread_item->idle;
    pthread_mutex_unlock(&thread_item->waitlock);
    return idle;
}

void thread_join(thread_t *thread_item)
{
    pthread_join(thread_item->threadinfo, NULL);
}

int thread_create(thread_t *thread_item, const thread_attr_t *attr,  void *(*start_routine) (void *), void *arg)
{
    return pthread_create(&thread_item->threadinfo, attr, start_routine, arg);
}  
