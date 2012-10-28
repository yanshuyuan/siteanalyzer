#ifndef _THREAD_SAFE_QUEUE_H
#define _THREAD_SAFE_QUEUE_H

#include <stdio.h>
#include <pthread.h>
#include "queue.h"

typedef struct _thread_safe_queue {
    pthread_mutex_t mutex_lock;
    queue_t queue;
} ts_queue_t;

void ts_queue_init(ts_queue_t *q);
void ts_queue_destroy(ts_queue_t *q);
void ts_queue_push(ts_queue_t *q, void *element, size_t bytes);
void ts_queue_top(ts_queue_t *q, void *element);
void ts_queue_pop(ts_queue_t *q);
const void *ts_queue_front(ts_queue_t *q);
void ts_queue_clear(ts_queue_t *q);
int ts_queue_size(ts_queue_t *q);

#endif
