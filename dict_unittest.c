#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hash.h"
#include "element.h"
#include "dict.h"

#define HASH_SIZE 1

typedef struct _vertex_info {
    int vid; /* vertex id */
    sds_t url; /* vertex url */
} vertex_info_t;

void printDict(dict_t *dict) {
    int i, s;
    bucket_elem_t *elem;
    for(i = 0; i < HASH_SIZE; i++) {
	elem = dict->table[i].next;
	while(elem != NULL) {
	    printf("bucket[%d]: \n", i);
	    char *key = (char *)elem->key;
	    char *value = (char *)elem->value;
	    for(s = 0; s < elem->klen; s++) {
		putchar(key[s]);
	    }
	    putchar('\n');
	    for(s = 0; s < elem->vlen; s++) {
		putchar(value[s]);
	    }
	    putchar('\n');
	    elem = elem->next;
	}
    }
    printf("---------------------------\n");
}

int main() 
{
    dict_t dict;
    dict_init(&dict, HASH_SIZE, sdbm_hash);

    char *s_elem = "www.baidu.com";
    int s_len = strlen(s_elem);
    dict_insert(&dict, s_elem, s_len, s_elem, s_len);

    int i_elem = 1945;
    dict_insert(&dict, &i_elem, sizeof(i_elem), &i_elem, sizeof(i_elem));
	
    char c_elem = 'o';
    dict_insert(&dict, &c_elem, sizeof(c_elem), &c_elem, sizeof(c_elem));

    char *key = "www.qq.com";
    int k_len = strlen(key);
    vertex_info_t *v_elem = (vertex_info_t *)malloc(sizeof(vertex_info_t) + s_len);
    v_elem->vid = 71;
    v_elem->url.len = s_len;
    memcpy(v_elem->url.buffer, s_elem, s_len);
    dict_insert(&dict, key, k_len, v_elem, sizeof(vertex_info_t) + s_len);
	
    int f_elem = 'o';

    void *find_elem;
    find_elem = dict_find(&dict, s_elem, s_len);

    assert(strcmp((char *)find_elem, s_elem) == 0);

    find_elem = dict_find(&dict, &i_elem, sizeof(i_elem));
    assert(*(int *)find_elem == i_elem);

    find_elem = dict_find(&dict, &c_elem, sizeof(c_elem));
    assert(*(char *)find_elem == c_elem );

    find_elem = dict_find(&dict, key, k_len);
    vertex_info_t *value = (vertex_info_t *) find_elem;
    assert(value->vid == 71);
    assert(value->url.len == s_len);
    assert(strcmp(value->url.buffer, s_elem) == 0);

    assert(dict_find(&dict, (void *)"www", strlen("www")) == NULL);
    assert(dict_find(&dict, (void *)&f_elem, sizeof(f_elem)) == NULL);

    dict_remove(&dict, s_elem, strlen(s_elem));
    dict_remove(&dict, &i_elem, sizeof(i_elem));

    dict_insert(&dict, &i_elem, sizeof(i_elem), &i_elem, sizeof(i_elem));
    dict_insert(&dict, s_elem, strlen(s_elem), s_elem, strlen(s_elem));
    dict_insert(&dict, s_elem, strlen(s_elem), s_elem, strlen(s_elem));

    dict_destroy(&dict);
    return 0;
    
}
