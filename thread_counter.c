#include "thread_counter.h"

void thread_counter_init(thread_counter_t *thread_counter)
{
    thread_counter->count = 0;
    pthread_mutex_init(&thread_counter->lock, NULL);
}

void thread_counter_destroy(thread_counter_t *thread_counter)
{
    pthread_mutex_destroy(&thread_counter->lock);
}

void thread_counter_increase(thread_counter_t *thread_counter)
{
    pthread_mutex_lock(&thread_counter->lock);
    thread_counter->count++;
    pthread_mutex_unlock(&thread_counter->lock);
}

void thread_counter_decrease(thread_counter_t *thread_counter)
{
    pthread_mutex_lock(&thread_counter->lock);
    thread_counter->count--;
    pthread_mutex_unlock(&thread_counter->lock);
}

int thread_counter_get(thread_counter_t *thread_counter)
{
    int c; 
    pthread_mutex_lock(&thread_counter->lock);
    c = thread_counter->count;
    pthread_mutex_unlock(&thread_counter->lock);
    return c;
}

void thread_counter_set(thread_counter_t *thread_counter, int count)
{
    pthread_mutex_lock(&thread_counter->lock);
    thread_counter->count = count;
    pthread_mutex_unlock(&thread_counter->lock);

}

int thread_counter_compare(thread_counter_t *thread_counter, int count)
{
    int r;
    pthread_mutex_lock(&thread_counter->lock);
    r = thread_counter->count - count;
    pthread_mutex_unlock(&thread_counter->lock);
    return r;
}

