#ifndef _ELEMENT_H
#define _ELEMENT_H

typedef struct _elem {
    struct _elem *next;
    int len;
    char buffer[];
} elem_t;

typedef struct _sds {
    int len;
    char buffer[];
} sds_t;

#endif
