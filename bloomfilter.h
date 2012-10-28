#ifndef __BLOOMFILTER_H__
#define __BLOOMFILTER_H__

#include "hash.h"
#include <malloc.h>
#include <limits.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdarg.h>



typedef struct {
    unsigned int bsize;
    char *bits;
    unsigned int fsize;
    hashfunc_t *func;
}BF;

int bloom_init(BF *bf, unsigned int bsize, unsigned int fsize, ...);
int bloom_add(BF *bf, char *str);
int bloom_check(BF *bf, char *str);
int bloom_destroy(BF *bf);

#endif
