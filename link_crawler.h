#ifndef _CRAWLER_H_
#define _CRAWLER_H_

#include <stdio.h>
#include <stdlib.h>
#include "link_parser.h"
#include "list.h"
#include "http_url.h"
#include "http_protocol.h"
#include "http_client.h"

#define CRAWLER_OK 0
#define CRAWLER_NONEED 1
#define CRAWLER_FAILED 2
#define CRAWLER_BREAK 3
#define CRAWLER_UNREACH 4
#define CRAWLER_UNKNOWN 5
#define CRAWLER_TIMEOUT 6
#define CRAWLER_NULL 7

typedef struct _link_crawler {
    http_client_t http_client;
} link_crawler_t;

int crawler_crawl(link_crawler_t *crawler, char *url, list_t *link_list); 

#endif

