/*****
** list.c
** - implements the methods declared in list.h
*/
#include "list.h"

void list_init(list_t *l) 
{
    l->meta.len = 0;
    l->meta.next = NULL;
    l->head = &l->meta;
    l->tail = l->head;
}

void list_destroy(list_t *l)
{
    list_clear(l);
}

void list_push_back(list_t *l, void *element, size_t bytes) 
{
/*
    printf("=============list_push_back malloc size: %d================\n", sizeof(elem_t) + bytes);
*/
    elem_t *_n_elem = (elem_t *)malloc(sizeof(elem_t) + bytes);
    if(_n_elem == NULL) {
	fprintf(stderr, "Error: allocate memory failed in func 'list_push_back'.\n");
    } else {
        _n_elem->len = bytes;
        memcpy(_n_elem->buffer, element, bytes);
        _n_elem->next = NULL;
        l->tail->next = _n_elem;
        l->tail = l->tail->next;
        l->meta.len++;
    }
}

void list_push_front(list_t *l, void *element, size_t bytes)
{
    elem_t *_n_elem = (elem_t *)malloc(sizeof(elem_t) + bytes);
    if(_n_elem == NULL) {
	fprintf(stderr, "Error: allocate memory failed in func 'list_push_front'.\n");
    } else {
        _n_elem->len = bytes;
        memcpy(_n_elem->buffer, element, bytes);
        _n_elem->next = NULL;
	_n_elem->next = l->head->next;
	l->head->next = _n_elem;
        l->meta.len++;
    }
}


elem_t *_list_index_of(list_t *l, size_t index)
{
    if(index >= 0 && index < l->meta.len) {
        size_t ix = 0;
        elem_t *_c_elem = l->head->next;

        while(_c_elem != NULL) {
            if(index == ix) {
                return _c_elem;
            } else {
                _c_elem = _c_elem->next;
                ix++;
            }
        }

    } else {
	fprintf(stderr, "Error: index out of list in func '_list_index_of'.\n");
    }
    return NULL;
}

void *list_index_of(list_t *l, size_t index)
{
    elem_t *elem = _list_index_of(l, index);
    return elem->buffer;
}

void list_erase(list_t *l, size_t index)
{
    if(index >= 0 && index < l->meta.len) {
        size_t ix = 0;
        elem_t *_c_elem = l->head->next;
        elem_t *_p_elem = l->head;
        while(_c_elem != NULL) {
            if(index == ix) {
		if(index == l->meta.len - 1) { l->tail = _p_elem; }
                _p_elem->next = _c_elem->next;
                l->meta.len--;
                free(_c_elem);
		return;
            } else {
                _p_elem = _c_elem;
                _c_elem = _c_elem->next;
		ix++;
            }
        }
    } else {
	fprintf(stderr, "Error: index out of list in func 'list_erase'.\n");
    }
}

void list_insert(list_t *l, size_t index, void *element, size_t bytes)
{
    if(index == 0) {
	list_push_front(l, element, bytes);
    } else if(index == l->meta.len) {
	list_push_back(l, element, bytes);
    } else {
        elem_t *_f_elem = _list_index_of(l, index - 1);
        elem_t *_n_elem = (elem_t *)malloc(sizeof(elem_t) + bytes);
	if(_n_elem != NULL) {
	    _n_elem->len = bytes;
            memcpy(_n_elem->buffer, element, bytes);
            _n_elem->next = _f_elem->next;
            _f_elem->next = _n_elem;
            l->meta.len++;
	} else {
	    fprintf(stderr, "Error: allocate memory failed in func 'list_insert'.\n");
        }
    }
}

void list_clear(list_t *l)
{
    while(l->meta.len != 0) {
	list_erase(l, 0);
    }
}

int list_size(list_t *l)
{
    return l->meta.len;
}

elem_t* list_first(list_t *l) 
{
    return l->head->next;
}

elem_t* list_end(list_t *l) 
{
    return NULL;
}

elem_t* list_next(elem_t *iter) 
{
    return iter == NULL ? NULL : iter->next;
}



