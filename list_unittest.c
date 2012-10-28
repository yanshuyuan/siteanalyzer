#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

void printList(list_t *l) 
{
    elem_t *iter;
    int i;
    printf("=============================================\n");
    for(iter = list_first(l); iter != list_end(l); iter = list_next(iter)) {
	printf("====ptr: %p============\n", iter);
	printf("bytes: %d\n", iter->len);
	for(i = 0; i < iter->len; i++) {
	    putchar(iter->buffer[i]);
	}
	putchar('\n');
    }
    printf("=============================================\n");
}

int main() {
    
    list_t *url_list = (list_t *)malloc(sizeof(list_t));
    list_init(url_list);
    assert(list_size(url_list) == 0);
    printf("----------------init---------------------\n");
    printList(url_list);

    
    char c_elem = '1';
    list_push_back(url_list, &c_elem, sizeof(char));
    assert(list_size(url_list) == 1);
    printf("----------------push back '1'---------------------\n");
    printList(url_list);

    char *s_elem = "www.baidu.com";
    list_push_front(url_list, s_elem, strlen(s_elem));
    printf("----------------push front \"www.baidu.com\"---------------------\n");
    assert(list_size(url_list) == 2);
    printList(url_list);
    
    void *pop_elem;
    pop_elem = list_index_of(url_list, 1);
    assert(*(char *)pop_elem == c_elem);

    int i_elem = 65536;
    list_insert(url_list, 0, &i_elem, sizeof(int));
    printf("----------------insert [0] 65536---------------------\n");
    printList(url_list);
    assert(list_size(url_list) == 3);

    pop_elem = list_index_of(url_list, 1);
    assert(strcmp((char *)pop_elem, s_elem) == 0);

    elem_t *st_elem = (elem_t *)malloc(sizeof(elem_t) + strlen(s_elem));;
    st_elem->len = strlen(s_elem);
    st_elem->next = st_elem;
    memcpy(st_elem->buffer, s_elem, strlen(s_elem));
    list_insert(url_list, 2, st_elem, sizeof(elem_t) + strlen(s_elem));
    printf("----------------insert [2] struct---------------------\n");
    printList(url_list);
    assert(list_size(url_list) == 4);
   
    pop_elem = list_index_of(url_list, 2);
    printf("%s\n", ((elem_t *)pop_elem)->buffer);
    printf("%d\n", ((elem_t *)pop_elem)->len);
    printf("%d\n", strlen(s_elem)); 
    assert(((elem_t *)pop_elem)->len == strlen(s_elem));
    assert(strcmp(((elem_t *)pop_elem)->buffer, s_elem) == 0);
    assert(((elem_t *)pop_elem)->next ==  st_elem);
    free(st_elem);

    
    list_erase(url_list, 2);
    printf("----------------erase [2]--------------------\n");
    printList(url_list);

    pop_elem = list_index_of(url_list, 0);
    assert(*(int *)pop_elem == i_elem);
    
    list_erase(url_list, 2);
    printf("----------------erase [2]--------------------\n");
    printList(url_list);
    assert(list_size(url_list) == 2);
	
    list_clear(url_list);
    printf("----------------clear--------------------\n");
    printList(url_list);
    assert(list_size(url_list) == 0);

    list_destroy(url_list);

    free(url_list);
    printf("----------------pass--------------------\n");
}
