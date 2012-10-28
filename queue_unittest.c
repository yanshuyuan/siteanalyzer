#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "queue.h"

int main() {
    /*test for queue_init*/
    queue_t *url_queue = (queue_t *)malloc(sizeof(queue_t));
    queue_init(url_queue);
    assert(queue_size(url_queue) == 0);

    /*test for queue_push*/
    char c_elem = '1';
    queue_push(url_queue, &c_elem, sizeof(char));
    assert(queue_size(url_queue) == 1);
    
    char *s_elem = "www.baidu.com";
    queue_push(url_queue, s_elem, strlen(s_elem));
    assert(queue_size(url_queue) == 2);
    
    int i_elem = 65536;
    queue_push(url_queue, &i_elem, sizeof(int));
    assert(queue_size(url_queue) == 3);

    elem_t *st_elem = (elem_t *)malloc(sizeof(elem_t) + strlen(s_elem));;
    st_elem->len = strlen(s_elem);
    st_elem->next = st_elem;
    memcpy(st_elem->buffer, s_elem, strlen(s_elem));
    queue_push(url_queue, st_elem, sizeof(elem_t) + strlen(s_elem));
    assert(queue_size(url_queue) == 4);
  
    /*test for queue_front in type char*/
    void *pop_elem;
    pop_elem = queue_front(url_queue);
    assert(*(char *)pop_elem == c_elem);
    
    /*test for queue_pop in type char*/
    queue_pop(url_queue);
    assert(queue_size(url_queue) == 3);
    
    /*test for queue_front in type string*/
    pop_elem = queue_front(url_queue);
    assert(strcmp((char *)pop_elem, s_elem) == 0);
    
    /*test for queue_pop in type string*/
    queue_pop(url_queue);
    assert(queue_size(url_queue) == 2);
    
    /*test for queue_front in type int*/
    pop_elem = queue_front(url_queue);
    assert(*(int *)pop_elem == i_elem);

    /*test for queue_pop in type int*/       	
    queue_pop(url_queue);
    assert(queue_size(url_queue) == 1);

    /*test for queue_front in type struct*/
    pop_elem = queue_front(url_queue);
    assert(((elem_t *)pop_elem)->len == strlen(s_elem));
    assert(((elem_t *)pop_elem)->next == st_elem);
    assert(strcmp(((elem_t *)pop_elem)->buffer, s_elem) == 0);

    queue_pop(url_queue);
    assert(queue_size(url_queue) == 0);
    /* test push after all element pop */
    queue_push(url_queue, &c_elem, sizeof(char));
    assert(queue_size(url_queue) == 1);
    
    queue_push(url_queue, s_elem, strlen(s_elem));
    assert(queue_size(url_queue) == 2);
    
    queue_push(url_queue, &i_elem, sizeof(int));
    assert(queue_size(url_queue) == 3);

    queue_push(url_queue, st_elem, sizeof(elem_t) + strlen(s_elem));
    assert(queue_size(url_queue) == 4);

    pop_elem = queue_front(url_queue);
    assert(*(char *)pop_elem == c_elem);
    
    /*test for queue_pop in type char*/
    queue_pop(url_queue);
    assert(queue_size(url_queue) == 3);
    
    /*test for queue_front in type string*/
    pop_elem = queue_front(url_queue);
    assert(strcmp((char *)pop_elem, s_elem) == 0);
    
    /*test for queue_pop in type string*/
    queue_pop(url_queue);
    assert(queue_size(url_queue) == 2);
    
    /*test for queue_front in type int*/
    pop_elem = queue_front(url_queue);
    assert(*(int *)pop_elem == i_elem);

    /*test for queue_pop in type int*/       	
    queue_pop(url_queue);
    assert(queue_size(url_queue) == 1);

    /*test for queue_front in type struct*/
    pop_elem = queue_front(url_queue);
    assert(((elem_t *)pop_elem)->len == strlen(s_elem));
    assert(((elem_t *)pop_elem)->next == st_elem);
    assert(strcmp(((elem_t *)pop_elem)->buffer, s_elem) == 0);

    /*test for queue_clear*/
    queue_clear(url_queue);
    assert(queue_size(url_queue) == 0);
    
    queue_destroy(url_queue);

    free(st_elem);
    free(url_queue);
}
