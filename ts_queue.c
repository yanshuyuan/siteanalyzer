#include "ts_queue.h"

void ts_queue_init(ts_queue_t *ts_queue)
{
    pthread_mutex_init(&ts_queue->mutex_lock, NULL);
    queue_init(&ts_queue->queue);
}

void ts_queue_destroy(ts_queue_t *ts_queue)
{
    pthread_mutex_destroy(&ts_queue->mutex_lock);
    queue_destroy(&ts_queue->queue);
}

void _queue_lock(ts_queue_t *ts_queue)
{
    pthread_mutex_lock(&ts_queue->mutex_lock);
}

void _queue_unlock(ts_queue_t *ts_queue)
{
    pthread_mutex_unlock(&ts_queue->mutex_lock);
}

void ts_queue_push(ts_queue_t *ts_queue, void *element, size_t bytes)
{
    _queue_lock(ts_queue); 
    queue_push(&ts_queue->queue, element, bytes);
    _queue_unlock(ts_queue); 
}

void ts_queue_pop(ts_queue_t *ts_queue)
{
    _queue_lock(ts_queue); 
    queue_pop(&ts_queue->queue);
    _queue_unlock(ts_queue); 
}

const void *ts_queue_front(ts_queue_t *ts_queue)
{
    const void *elem = NULL;
    _queue_lock(ts_queue); 
    elem = queue_front(&ts_queue->queue);
    _queue_unlock(ts_queue); 
    return elem;
}

void ts_queue_top(ts_queue_t *ts_queue, void *element)
{
    _queue_lock(ts_queue); 
    queue_top(&ts_queue->queue, element);
    _queue_unlock(ts_queue); 
}

void ts_queue_clear(ts_queue_t *ts_queue)
{
    _queue_lock(ts_queue);
    queue_clear(&ts_queue->queue);
    _queue_unlock(ts_queue);
}

int ts_queue_size(ts_queue_t *ts_queue)
{
    int size;
    _queue_lock(ts_queue);
    size = queue_size(&ts_queue->queue);
    _queue_unlock(ts_queue);
    return size;
}
