#ifndef __DICT_H
#define __DICT_H

#include <malloc.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "hash.h"


typedef struct _bucket_elem {
    struct _bucket_elem *next;
    int klen;
    void *key;
    int vlen;
    void *value;
    char buffer[];
} bucket_elem_t;

typedef struct _bucket {
    int size; 
    bucket_elem_t *next;
} bucket_t;

typedef struct _dict {
    size_t size;
    bucket_t *table;
    hashfunc_t hash_func;
} dict_t;

void dict_init(dict_t *dict, size_t size, hashfunc_t hash_func);
void dict_destroy(dict_t *dict);

void *dict_insert(dict_t *dict, void *key, size_t kbytes, void *value, size_t vbytes);
void dict_remove(dict_t *dict, void *key, size_t bytes);
void *dict_find(dict_t *dict, void *key, size_t bytes);

unsigned int _dict_index(dict_t *dict, void *key, size_t bytes);

void _bucket_remove(bucket_t *bucket, void *key, size_t bytes);
bucket_elem_t *_bucket_insert(bucket_t *bucket, void *key, size_t kbytes, void *value, size_t vbytes);
bucket_elem_t *_bucket_find(bucket_t *bucket, void *key, size_t bytes);

#endif
