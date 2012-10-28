#ifndef _THREAD_COUNTER_H
#define _THREAD_COUNTER_H

#include <pthread.h>

typedef struct _thread_count {
    int count;
    pthread_mutex_t lock;
} thread_counter_t;

void thread_counter_init(thread_counter_t *thread_counter);
void thread_counter_increase(thread_counter_t *thread_counter);
void thread_counter_decrease(thread_counter_t *thread_counter);
int thread_counter_get(thread_counter_t *thread_counter);
void thread_counter_set(thread_counter_t *thread_counter, int count);
int thread_counter_compare(thread_counter_t *thread_counter, int count);
void thread_counter_destroy(thread_counter_t *thread_counter);


#endif
