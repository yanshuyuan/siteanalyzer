#include "http_protocol.h"

void http_request_init(http_req_t *request)
{
    bzero(request, sizeof(http_req_t));
    request->enti_body.len = 0;
    request->enti_body.used = 0;
}

void http_response_init(http_res_t *response)
{
    bzero(response, sizeof(http_res_t));
    response->enti_body.len = 0;
    response->enti_body.used = 0;
}

void http_request_destroy(http_req_t *request)
{
    if(request->enti_body.len != 0) {
	free(request->enti_body.buffer);
	bzero(request, sizeof(http_req_t));
	request->enti_body.len = 0;
        request->enti_body.used = 0;
    }
}

void http_response_destroy(http_res_t *response)
{
    if(response->enti_body.len != 0) {
	free(response->enti_body.buffer);
	bzero(response, sizeof(http_res_t));
    	response->enti_body.len = 0;
        response->enti_body.used = 0;
    }
}

#ifndef __HTTP_HEADER_APPEND
#define __HTTP_HEADER_APPEND(_dest, _attr, _value) {\
    if(_value != NULL && strlen(_value) != 0) { \
	strcat(_dest, _attr); \
	strcat(_dest, _value); \
        strcat(_dest, "\r\n"); \
    }\
}
#endif

int http_response_header(http_res_t *res_s, char *header)
{
    char attr[4096], value[4096];
    bzero(attr, 4096);
    bzero(value, 4096);
    sscanf(header, "%[^:]: %[^\n]", attr, value);

#ifndef __HTTP_HEADER_MATCH_APPEND
#define __HTTP_HEADER_MATCH_APPEND(_dest, _attr, _value, _standard) \
    if(strcmp(_attr, _standard) == 0) { strcpy(_dest, _value); _dest[strlen(_dest) - 1] = '\0'; return 1;}
#endif
/*
    __HTTP_HEADER_MATCH_APPEND(res_s->gen_header.date, attr, value, "Date");
    __HTTP_HEADER_MATCH_APPEND(res_s->gen_header.pragma, attr, value, "Parama");
    __HTTP_HEADER_MATCH_APPEND(res_s->gen_header.connection, attr, value, "Connection");
    __HTTP_HEADER_MATCH_APPEND(res_s->gen_header.cache_control, attr, value, "Cache-Control");
    __HTTP_HEADER_MATCH_APPEND(res_s->gen_header.trailer, attr, value, "Trailer");
    __HTTP_HEADER_MATCH_APPEND(res_s->gen_header.transfer_encoding, attr, value, "Transfer-Encoding");
    __HTTP_HEADER_MATCH_APPEND(res_s->gen_header.upgrade, attr, value, "Upgrade");
    __HTTP_HEADER_MATCH_APPEND(res_s->gen_header.via, attr, value, "Via");
    __HTTP_HEADER_MATCH_APPEND(res_s->gen_header.warning, attr, value, "Warning");

    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.allow, attr, value, "Allow");
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.content_encoding, attr, value, "Content-Encoding");
*/
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.content_length, attr, value, "Content-Length");
/*
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.content_type, attr, value, "Content-Type");
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.expires, attr, value, "Expires");
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.last_modified, attr, value, "Last-Modified");
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.extension_header, attr, value, "Extension-Header");
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.content_location, attr, value, "Content-Location");
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.content_language, attr, value, "Content-Language");
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.content_md5, attr, value, "Content-MD5");
    __HTTP_HEADER_MATCH_APPEND(res_s->enti_header.content_range, attr, value, "Content-Range");

    __HTTP_HEADER_MATCH_APPEND(res_s->res_header.location, attr, value, "Location");
*/
/*
    __HTTP_HEADER_MATCH_APPEND(res_s->res_header.server, attr, value, "Server");
    __HTTP_HEADER_MATCH_APPEND(res_s->res_header.www_authenticate, attr, value, "WWW-Authenticate");
    __HTTP_HEADER_MATCH_APPEND(res_s->res_header.accept_ranges, attr, value, "Accept-Ranges");
    __HTTP_HEADER_MATCH_APPEND(res_s->res_header.age, attr, value, "Age");
    __HTTP_HEADER_MATCH_APPEND(res_s->res_header.etag, attr, value, "Etag");
    __HTTP_HEADER_MATCH_APPEND(res_s->res_header.proxy_authenticate, attr, value, "Proxy-Authenticate");
    __HTTP_HEADER_MATCH_APPEND(res_s->res_header.retry_after, attr, value, "Retry-After");
    __HTTP_HEADER_MATCH_APPEND(res_s->res_header.vary, attr, value, "Vary");
*/

    return 0;
}

int http_create_request(http_req_t *req_s, char *request) 
{
    /* request line*/
    sprintf(request, "%s %s %s\r\n", req_s->req_line.method, 
		req_s->req_line.request_uri, req_s->req_line.http_version);

    /* request header */
    
    /* general header field*/
    __HTTP_HEADER_APPEND(request, "Date: ", req_s->gen_header.date);
/*
    __HTTP_HEADER_APPEND(request, "Pragma: ", req_s->gen_header.pragma);
    __HTTP_HEADER_APPEND(request, "Connection: ", req_s->gen_header.connection);
    __HTTP_HEADER_APPEND(request, "Cache-Control: ", req_s->gen_header.cache_control);
    __HTTP_HEADER_APPEND(request, "Trailer: ", req_s->gen_header.trailer);
    __HTTP_HEADER_APPEND(request, "Transfer-Encoding: ", req_s->gen_header.transfer_encoding);
    __HTTP_HEADER_APPEND(request, "Upgrade: ", req_s->gen_header.upgrade);
    __HTTP_HEADER_APPEND(request, "Via: ", req_s->gen_header.via);
    __HTTP_HEADER_APPEND(request, "Warning: ", req_s->gen_header.warning);
*/

    /* request header field*/
/*
    __HTTP_HEADER_APPEND(request, "Authorization: ", req_s->req_header.authorization);
    __HTTP_HEADER_APPEND(request, "From: ", req_s->req_header.from);
    __HTTP_HEADER_APPEND(request, "If-Modified-Since: ", req_s->req_header.if_modified_since);
    __HTTP_HEADER_APPEND(request, "Refer: ", req_s->req_header.referer);
*/
    __HTTP_HEADER_APPEND(request, "User-Agent: ", req_s->req_header.user_agent);
    __HTTP_HEADER_APPEND(request, "Host: ", req_s->req_header.host);
    __HTTP_HEADER_APPEND(request, "Accept: ", req_s->req_header.accept);
/*
    __HTTP_HEADER_APPEND(request, "Accept-Charset: ", req_s->req_header.accept_charset);
    __HTTP_HEADER_APPEND(request, "Accept-Encoding: ", req_s->req_header.accept_encoding);
    __HTTP_HEADER_APPEND(request, "Accept-Language: ", req_s->req_header.accept_language);
    __HTTP_HEADER_APPEND(request, "Expect: ", req_s->req_header.expect);
    __HTTP_HEADER_APPEND(request, "If-Match: ", req_s->req_header.if_match);
    __HTTP_HEADER_APPEND(request, "If-None-Match: ", req_s->req_header.if_none_match);
    __HTTP_HEADER_APPEND(request, "If-Range: ", req_s->req_header.if_range);
    __HTTP_HEADER_APPEND(request, "If-Unmodified-Since: ", req_s->req_header.if_unmodified_since);
    __HTTP_HEADER_APPEND(request, "Max-Forwards: ", req_s->req_header.max_forwards);
    __HTTP_HEADER_APPEND(request, "Proxy-Authorization: ", req_s->req_header.proxy_authorization);
    __HTTP_HEADER_APPEND(request, "Range: ", req_s->req_header.range);
    __HTTP_HEADER_APPEND(request, "TE: ", req_s->req_header.te);
*/

    /* entity header field*/
/*
    __HTTP_HEADER_APPEND(request, "Allow: ", req_s->enti_header.allow);
    __HTTP_HEADER_APPEND(request, "Content-Encoding: ", req_s->enti_header.content_encoding);
*/
    __HTTP_HEADER_APPEND(request, "Content-Length: ", req_s->enti_header.content_length);

/*
    __HTTP_HEADER_APPEND(request, "Expires: ", req_s->enti_header.expires);
    __HTTP_HEADER_APPEND(request, "Last-Modified: ", req_s->enti_header.last_modified);
    __HTTP_HEADER_APPEND(request, "Extension-Header: ", req_s->enti_header.extension_header);
    __HTTP_HEADER_APPEND(request, "Content-Location: ", req_s->enti_header.content_location);
    __HTTP_HEADER_APPEND(request, "Content-Language: ", req_s->enti_header.content_language);
    __HTTP_HEADER_APPEND(request, "Content-MD5: ", req_s->enti_header.content_md5);
    __HTTP_HEADER_APPEND(request, "Content-Range: ", req_s->enti_header.content_range);
*/

    /* blank line */
    strcat(request, "\r\n");

    /* request entity */
    __HTTP_HEADER_APPEND(request, "", req_s->enti_body.buffer);
    return 0;
}
