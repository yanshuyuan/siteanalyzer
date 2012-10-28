#ifndef _QUEUE_H
#define _QUEUE_H

#include "element.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _queue {
    elem_t meta;
    elem_t *head;
    elem_t *tail;
} queue_t;

/*****
** initialize an empty queue_t
** must be called first after a new queue_t is declared
*/ void queue_init(queue_t *q);
   void queue_destroy(queue_t *q);

/*****
** push a new element to the end of the queue_t
** it's up to the client code to allocate and maintain memory of "element"
*/ void queue_push(queue_t *q, const void *element, size_t bytes);

/*****
** return the first element in the queue_t, or NULL when the queue_t is empty
*/ const void *queue_front(queue_t *q);

 void queue_top(queue_t *q, void *element);

/*****
** remove the first element (pointer) from the queue_t
** set "release" to non-zero if memory deallocation is desired
*/ void queue_pop(queue_t *q);

/*****
** remove all elements (pointers) from the queue_t
** set "release" to non-zero if memory deallocation is desired
*/ void queue_clear(queue_t *q);

/*****
** return current number of elements in the queue_t, or 0 when queue_t is empty
*/ int queue_size(queue_t *q);

#endif /* _RZ_C_QUEUE_ */
