#include "dict.h"

void dict_init(dict_t *dict, size_t size, hashfunc_t hash_func)
{
/*
    printf("--------dict init malloc size: %d------------------\n", sizeof(bucket_t) * size);
*/
    if((dict->table = (bucket_t *) malloc(sizeof(bucket_t) * size)) == NULL) {
	fprintf(stderr, "Error: allocate memory failed in func 'dict_init'.\n");
    } else {
	dict->size = size;
	dict->hash_func = hash_func;
	int i;
	for(i = 0; i < dict->size; i++) {
	    dict->table[i].size = 0;
	    dict->table[i].next = NULL;
	}
    }
}

void dict_destroy(dict_t *dict)
{
    size_t i = 0;
    bucket_elem_t *_c_elem;
    for(i = 0; i < dict->size; i++) {
	_c_elem = dict->table[i].next;
	while(_c_elem != NULL && dict->table[i].size > 0) {
	    dict->table[i].next = _c_elem->next;
	    free(_c_elem);
	    dict->table[i].size--;
	    _c_elem = dict->table[i].next;
	}
    }

    free(dict->table);
}

unsigned int _dict_index(dict_t *dict, void *key, size_t bytes)
{
    return dict->hash_func((const char *)key, bytes) % dict->size;
}

bucket_elem_t *_bucket_insert(bucket_t *bucket, void *key, size_t kbytes, void *value, size_t vbytes)
{
    bucket_elem_t *_n_elem = NULL;
/*
    printf("--------bucket_insert malloc size: %d------------------\n", sizeof(bucket_elem_t) + kbytes + vbytes);
*/
    if((_n_elem = (bucket_elem_t *) malloc(sizeof(bucket_elem_t) + kbytes + vbytes)) != NULL) {
	_bucket_remove(bucket, key, kbytes);
	/* let _n_elem->key and _n_elem->value pointer to buffer */
	_n_elem->klen = kbytes;
	_n_elem->vlen = vbytes;
	_n_elem->key = _n_elem->buffer;
	_n_elem->value = _n_elem->buffer + _n_elem->klen;
	/* copy key */
	memcpy(_n_elem->key, key, kbytes);
	/* copy value */
	memcpy(_n_elem->value, value, vbytes);
	/* add bucet_elem into bucket */
	_n_elem->next = bucket->next;
	bucket->next = _n_elem;
	bucket->size++;
    } else {
	fprintf(stderr, "Error: allocate memory failed in func '_bucket_insert'.\n");
    }
    return _n_elem;
}

void *dict_insert(dict_t *dict, void *key, size_t kbytes, void *value, size_t vbytes)
{
    unsigned int index = _dict_index(dict, key, kbytes);
    /* not founded in the index-th bucket, then insert */
    bucket_elem_t *elem = _bucket_insert(&dict->table[index], key, kbytes, value, vbytes);
    return elem == NULL ? NULL : elem->value;
}

bucket_elem_t *_bucket_find(bucket_t *bucket, void *key, size_t bytes)
{
    bucket_elem_t *_c_elem = bucket->next;
    while(_c_elem != NULL) {
	if(_c_elem->klen == bytes 
		&& memcmp(_c_elem->buffer, key, bytes) == 0) {
	    return _c_elem;
	} else {
	    _c_elem = _c_elem->next;
	}
    }
    return NULL;
}

void *dict_find(dict_t *dict, void *key, size_t bytes)
{
    unsigned int index = _dict_index(dict, key, bytes);
    /* not founded in the index-th bucket, then insert */
    bucket_elem_t *elem = _bucket_find(&dict->table[index], key, bytes);
    return elem == NULL ? NULL : elem->value;
}

void _bucket_remove(bucket_t *bucket, void *key, size_t bytes)
{
    bucket_elem_t *_c_elem = bucket->next;
    bucket_elem_t *_p_elem = NULL;
    while(_c_elem != NULL) {
	if(_c_elem->klen == bytes
		&& memcmp(_c_elem->buffer, key, bytes) == 0) {
	    if(_c_elem == bucket->next) {
		bucket->next = _c_elem->next;
	    } else {
	        _p_elem->next = _c_elem->next;
	    }
	    free(_c_elem);
	    return;
	} else {
	    _p_elem = _c_elem;
	    _c_elem = _c_elem->next;
	}
    }
}

void dict_remove(dict_t *dict, void *key, size_t bytes)
{
    unsigned int index = _dict_index(dict, key, bytes);
    _bucket_remove(&dict->table[index], key, bytes);
}
	

