#ifndef _LINKPARSER_H
#define _LINKPARSER_H

#include "list.h"

typedef struct _link_offset {
    int pre_so;
    int pre_eo;
    int path_so;
    int path_eo;
} link_offset_t;

void extract_link(const char *htmltxt, list_t *l);
void extract_absolute_link(const char *htmltxt, list_t *l, char *url);
void extract_absolute_link_s(const char *htmltxt, list_t *l, char *url);

#endif

