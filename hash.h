#ifndef __HASH_H__
#define __HASH_H__

#include <stdlib.h>

typedef unsigned int (*hashfunc_t) (const char *, size_t bytes);

unsigned int sdbm_hash(const char *, size_t bytes);
unsigned int sax_hash(const char *, size_t bytes);
unsigned int elf_hash(const char *, size_t bytes);
unsigned int djb_hash(const char *, size_t bytes);
unsigned int bkdr_hash(const char *, size_t bytes);
unsigned int js_hash(const char *, size_t bytes);

#endif
