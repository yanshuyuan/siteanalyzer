#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>
#include "thread_counter.h"

typedef struct _thread {
    pthread_t threadinfo;
    pthread_mutex_t waitlock; 
    thread_counter_t job_done;
    int id;
    int idle; 
} thread_t;

typedef pthread_attr_t thread_attr_t;

int thread_create(thread_t *thread_item, const thread_attr_t *attr,  void *(*start_routine) (void *), void *arg);
void thread_init(thread_t *thread_item);
void thread_destroy(thread_t *thread_item);
void thread_set_idle(thread_t *thread_item, int idle);
int thread_get_idle(thread_t *thread_item);
void thread_join(thread_t *thread_item);

#endif

