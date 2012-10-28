#include"hash.h"

unsigned int sax_hash(const char *key, size_t bytes)
{
    unsigned int h=0;

    while(bytes--) {
	h ^= (h<<5)+(h>>2)+(unsigned char)*key++;
    }

    return h;
}

unsigned int sdbm_hash(const char *key, size_t bytes)
{
    unsigned int h=0;
    while(bytes--) {
	 h=(unsigned char)*key++ + (h<<6) + (h<<16) - h;
    }
    return h;
}
unsigned int elf_hash(const char *str, size_t bytes)
{
    unsigned int hash = 0;
    unsigned int x = 0;
    while (bytes--) {
        hash = (hash << 4) + (*str++);
        if((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
            hash &= ~x;
        } 
    } 
    return hash;
} 

unsigned int djb_hash(const char * str, size_t bytes)
{
    unsigned int hash = 5381 ;
    while(bytes--) {
        hash += (hash << 5) + (*str++ ); 
    }
    return hash;
}


unsigned int bkdr_hash(const char * str, size_t bytes)
{
    unsigned int seed = 131 ; 
    unsigned int hash = 0 ;
    while(bytes--) {
        hash = hash * seed + (*str++);
    }
    return hash;
} 

unsigned int js_hash(const char *str, size_t bytes)
{
    unsigned int hash = 1315423911;
    while(bytes--) {
        hash^=((hash << 5) + (*str++) + (hash >> 2));
    }
    return hash;
} 
