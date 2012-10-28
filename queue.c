/*****
** queue.c
** - implements the methods declared in queue.h
*/

#include "queue.h"

/***
the queue module define a queue having a head node which record the queue length
***/
void queue_init(queue_t *q)
{
   q->meta.len = 0;
   q->meta.next = NULL;
   q->head = &q->meta;
   q->tail = q->head;
} 

void queue_destroy(queue_t *q)
{
   queue_clear(q);
}

int queue_size(queue_t *q)
{
    return q->meta.len;
}

void queue_push(queue_t *q, const void *element, size_t bytes)
{
/*
    printf("=================queue malloc size: %d==================\n", sizeof(elem_t) + bytes);
*/
    elem_t *_n_elem = (elem_t *)malloc(sizeof(elem_t) + bytes);
    if(_n_elem == NULL) {
	fprintf(stderr, "Error: allocate memory failed in func 'queue_push'.\n");
    } else {
        _n_elem->len = bytes;
        memcpy(_n_elem->buffer, element, bytes);
        _n_elem->next = NULL;
        q->tail->next = _n_elem;
        q->tail = q->tail->next;
        q->meta.len++;
    }
}

const void *queue_front(queue_t *q)
{
    return q->head->next->buffer;
}

void queue_pop(queue_t *q)
{
    elem_t *_p_elem = q->head->next; 
    q->head->next = _p_elem->next;
    free(_p_elem);
    q->meta.len--;
    if(q->meta.len == 0) { q->tail = &q->meta; }
}

void queue_top(queue_t *q, void *element)
{
    elem_t *_p_elem = q->head->next;
    memcpy(element, _p_elem->buffer, _p_elem->len);
    queue_pop(q);
}

void queue_clear(queue_t *q)
{
    while(q->head->next != NULL) {
        queue_pop(q);
    }
}
