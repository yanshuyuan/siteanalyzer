#ifndef _LIST_H
#define _LIST_H

#include "element.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>


typedef struct _list {
    elem_t meta;
    elem_t *head;
    elem_t *tail;
}list_t;

/*****
** initialize an empty list_t
** must be called first after a new list_t is declared
*/ void list_init(list_t *l);

/*****
** if you won't use list l, you must call list_destroy to free memory 
*/ void list_destroy(list_t *l);

/****
** list iterator func ***
*/
   elem_t* list_first(list_t *l);
   elem_t* list_end(list_t *l);
   elem_t* list_next(elem_t *l);

/*****
** push a new element to the end of the list_t
** it's up to the client code to allocate and maintain memory of "element"
*/ void list_push_back(list_t *l, void *element, size_t bytes);

/*****
** push a new element to the front of the list_t
** it's up to the client code to allocate and maintain memory of "element"
*/ void list_push_front(list_t *l, void *element, size_t bytes);

/*****
** insert the element (pointer) into the list_t as the index-th element 
** set "release" to non-zero if memory deallocation is desired
*/ void list_insert(list_t *l, size_t index, void *element, size_t bytes);


/*****
** return the index-th element in the list_t, or NULL when the list_t is empty
*/ void *list_index_of(list_t *l, size_t index);

/*****
** remove the index-th element (pointer) from the list_t
** set "release" to non-zero if memory deallocation is desired
*/ void list_erase(list_t *l, size_t index);


/*****
** remove all elements (pointers) from the list_t
** set "release" to non-zero if memory deallocation is desired
*/ void list_clear(list_t *l);

/*****
** return current number of elements in the list_t, or 0 when list_t is empty
*/ int list_size(list_t *l);

#endif


