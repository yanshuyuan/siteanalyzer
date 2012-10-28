#ifndef _URL_H
#define _URL_H

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>


/* the schema define as RFC 1738 */
/*
HTTP
httpurl        = "http[s]://" hostport [ "/" hpath [ "?" search ]]
hpath          = hsegment *[ "/" hsegment ]
hsegment       = *[ uchar | ";" | ":" | "@" | "&" | "=" ] //"." "_" added
search         = *[ uchar | ";" | ":" | "@" | "&" | "=" ] //"," "-" "." "+" "_"added
hostport: [a-zA-Z0-9][-a-zA-Z0-9]{0,62}(\.[a-zA-Z0-9][-a-zA-Z0-9]{0,62})+\.?[:[0-9]+]
*/

#define __DIGIT "0-9"
#define __CHAR "a-zA-Z"
#define __ALPHA __CHAR __DIGIT
#define __SEARCH "[-"__ALPHA";:@&=,_.+]*"
#define __HSEGMENT "[-"__ALPHA";:@&=_.+]*"
#define __HPATH __HSEGMENT"(/"__HSEGMENT")*"
#define __HOST "["__ALPHA"][-"__ALPHA"_]{0,62}(\\.["__ALPHA"][-"__ALPHA"_]{0,62})+\\.?"
#define __PORT "["__DIGIT"]+"
#define __HOSTPORT "("__HOST")(:("__PORT"))?"
#define __HTTPURL "^((["__ALPHA"]+)://)?"__HOSTPORT"((/"__HPATH")(\\?("__SEARCH"))?)?$"
#define __Z_HTTPURL "(((["__ALPHA"]+)://)?"__HOSTPORT")((/"__HPATH")(\\?("__SEARCH"))?)?"


typedef struct _http_url {
    char protocol[16];
    char host[64];
    char port[8];
    char path[256];
    char search[4096];
} http_url_t;

int http_url_parse(http_url_t *http_url, char *url);
int http_url_parse_s(http_url_t *http_url, char *url);
   
enum {URL_PROTO_UNRECOGNIZED, URL_HOST_UNRECOGNIZED, URL_PORT_UNRECOGNIZED, URL_PATH_UNRECOGNIZED, 
	URL_SEARCH_UNRECOGNIZED, URL_UNRECOGNIZED, URL_RECOGNIZED};

#endif
