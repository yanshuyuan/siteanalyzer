#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#include "http_protocol.h"
#include <malloc.h>
#include "network.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "http_protocol.h"



#define RESPONSE_OK 0
#define REQUEST_FAILED 1
#define RESPONSE_FAILED 2
#define RESPONSE_TIMEOUT 3

typedef struct _http_client {
    network_t connection;
    http_req_t request;
    http_res_t response;
} http_client_t;


typedef enum _content_t {HTTP_STATUS, HTTP_HEADER, HTTP_BODY} content_t;

int http_connect(http_client_t *http_client, char *hostname, int port);
void http_disconnect(http_client_t *http_client);
int http_do_get(http_client_t *http_client, char *path);
int http_response_status(http_client_t *http_client);
const char * http_response_body(http_client_t *http_client);

#endif

