#ifndef _WORKER_H_
#define _WORKER_H_

#include <malloc.h>
#include "link_crawler.h"
#include "ts_queue.h"
#include "thread_pool.h"
#include "webgraph.h"
#include "bloomfilter.h"
#include <string.h>

typedef struct _link_info {
    list_t *link_url;
    char *url;
} link_info_t; 

void *do_crawler(void *arg);
void *do_work(void *arg);

#endif
