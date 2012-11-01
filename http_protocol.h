#ifndef _HTTP_PROTOCOL_H
#define _HTTP_PROTOCOL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#define ENTITY_BODY_SIZE_DEFAULT 819200 /* 819200 */
#define HTTP_OK 200
#define HTTP_CREATED 201 
#define HTTP_ACCEPTED 202
#define HTTP_NO_CONTENT 204
#define HTTP_MOVED_PERMANENTLY 301
#define HTTP_MOVED_TEMPORARILY 302
#define HTTP_NOT_MODIFIELD 304
#define HTTP_BAD_REQUEST 400
#define HTTP_UNAUTHORIZED 401
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_SERVER_ERROR 500
#define HTTP_NOT_IMPLEMENTED 501
#define HTTP_BAD_GATEWAY 502
#define HTTP_SERVICE_UNAVAILABLE 503

/* below struct definition as http protocol 1.0 RFC 1945 */

typedef struct _http_request_line {
    char method[8];
    char request_uri[4096];
    char http_version[16];
} http_req_line_t;

typedef struct _http_response_line {
    char http_version[16];
    char status_code[8];
    char reason_phrase[64];
} http_res_line_t;

typedef struct _http_general_header {
    char date[128];
/*
    char pragma[128];
    char connection[128];
    char cache_control[128];
    char trailer[128];
    char transfer_encoding[128];
    char upgrade[128];
    char via[128];
    char warning[128];
*/
} http_gen_header_t;

typedef struct _http_entity_header {
/*
    char allow[128];
    char content_encoding[128];
*/
    char content_length[128];
    char content_type[128];
/*
    char expires[128];
    char last_modified[128];
    char extension_header[128];
    char content_location[128];
    char content_language[128];
    char content_md5[128];
    char content_range[128];
*/
} http_enti_header_t;

typedef struct _http_request_header {
/*
    char authorization[128];
    char from[128];
    char if_modified_since[128];
    char referer[128];
*/
    char user_agent[128];
    char host[128];
    char accept[128];
/*
    char accept_charset[128];
    char accept_encoding[128];
    char accept_language[128];
    char expect[128];
    char if_match[128];
    char if_none_match[128];
    char if_range[128];
    char if_unmodified_since[128];
    char max_forwards[128];
    char proxy_authorization[128];
    char range [128];
    char te[128];
*/
} http_req_header_t;

typedef struct _http_response_header {
/*
    char location [4096];
*/
    char server[128];
/*
    char www_authenticate[128];
    char accept_ranges[128];
    char age[128];
    char etag[128];
    char proxy_authenticate[128];
    char retry_after[128];
    char vary [128];
*/
} http_res_header_t;

typedef struct _http_entity_body {
    int used;
    int len;
    char *buffer;
} http_enti_body_t;

typedef struct _http_request {
    http_req_line_t req_line; /*request line*/
    http_gen_header_t gen_header;/*general header field*/
    http_req_header_t req_header;/*request header field*/
    http_enti_header_t enti_header;/*entity header field*/
    http_enti_body_t enti_body;/*request body fielde*/
} http_req_t;

typedef struct _http_response {
    http_res_line_t res_line;
    http_gen_header_t gen_header;
    http_res_header_t res_header;
    http_enti_header_t enti_header;/*entity header field*/
    http_enti_body_t enti_body;
} http_res_t;


void http_request_init(http_req_t *request);
void http_response_init(http_res_t *response);
void http_request_destroy(http_req_t *request);
void http_response_destroy(http_res_t *response);
int http_response_header(http_res_t *res_s, char *header);
int http_create_request(http_req_t *req_s, char *request);

#endif
