/*****
** http_client.c
** - implements the methods declared in http_client.h
** - 封装处理http协议
*/

#include "http_client.h"

#define REQ_SIZE 1024
#define BUFFER_SIZE 4096

int http_connect(http_client_t *http_client, char *hostname, int port)
{
    http_request_init(&http_client->request);
    http_response_init(&http_client->response);
    sprintf(http_client->request.req_header.host, "%s", hostname);

    return open_tcp(&http_client->connection, hostname, port);
}

void http_disconnect(http_client_t *http_client)
{
    http_request_destroy(&http_client->request);
    http_response_destroy(&http_client->response);
    close_tcp(&http_client->connection);
}

int http_do_get(http_client_t *http_client, char *path)
{
    char http_request[REQ_SIZE];
    int timeout, size; /* send or recv timeout more than 3 times will stop */

    http_req_t *request = &http_client->request;

    sprintf(request->req_line.method, "GET");
    sprintf(request->req_line.request_uri, "%s", path);
    sprintf(request->req_line.http_version, "HTTP/1.0");
    sprintf(request->req_header.user_agent, "Mozilla/5.0 (X11; Linux i686) AppleWebKit/536.5 "
			"(KHTML, like Gecko) Chrome/19.0.1084.52 Safari/536.5");
    sprintf(request->req_header.accept, "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    sprintf(request->enti_header.content_type, "application/x-www-form-urlencoded");
    
    bzero(http_request, REQ_SIZE);
    http_create_request(request, http_request);
    /* send http request */

    timeout = 3;
    do {
        size = nsend(&http_client->connection, http_request, strlen(http_request));
        timeout--;
    } while(size < 0 && timeout);

    if(size < 0) {
	fprintf(stderr, "Http Request failed, error code: %d\n", size);
	return REQUEST_FAILED;
    }

    char buf[BUFFER_SIZE];
    http_res_t *response = &http_client->response;
   
    int entity_body_size, recv_body_length = 0;
    
    content_t flag = HTTP_STATUS;
    /* recv http response */
    timeout = 3;
    do {
        switch(flag) {
	case HTTP_STATUS: /*recv status line */
	    flag = HTTP_HEADER;
	    bzero(buf, BUFFER_SIZE);
	    size = recv_line(&http_client->connection, buf, BUFFER_SIZE);
	    if(size > 0) {
		timeout = 3;
	        sscanf(buf, "%s %s %s", response->res_line.http_version, response->res_line.status_code, response->res_line.reason_phrase);
	    } else {
		switch(size) {
		case RECV_OVER:
		    return RESPONSE_OK;
		break;
		case RECV_TIMEOUT:
		    timeout--;
		    if(!timeout) return RESPONSE_TIMEOUT;
		break;
		case RECV_ERROR:
		    return RESPONSE_FAILED;
		break;
		}
	    }
	    break;
	case HTTP_HEADER:/*recv response header */
	    bzero(buf, BUFFER_SIZE);
	    size = recv_line(&http_client->connection, buf, BUFFER_SIZE);
	    if(size > 0) {
		timeout = 3;
	        if(strcmp(buf, "\r\n") == 0) {
	            entity_body_size = strlen(response->enti_header.content_length) != 0 
	            		? atoi(response->enti_header.content_length) + 100 : ENTITY_BODY_SIZE_DEFAULT;
	            /*
	            printf("===========================do_get malloc_size: %d=================================\n", entity_body_size);
	            */
	            response->enti_body.buffer = (char *) malloc(entity_body_size);
	            if(response->enti_body.buffer == NULL) {
	                response->enti_body.len = 0;
	                fprintf(stderr, "Error: allocate memory failed in func 'do_get'.\n");
	                return RESPONSE_FAILED;
	            } else {
	                response->enti_body.len = entity_body_size;
	                bzero(response->enti_body.buffer, entity_body_size);
                    }
	            flag = HTTP_BODY;
                }
	    	http_response_header(response, buf);
	    } else {
		switch(size) {
		case RECV_OVER:
		    return RESPONSE_OK;
		break;
		case RECV_TIMEOUT:
		    timeout--;
		    if(!timeout) return RESPONSE_TIMEOUT;
		break;
		case RECV_ERROR:
		    return RESPONSE_FAILED;
		break;
		}
	    }
	    break;
	case HTTP_BODY: /*recv entity body */
	    bzero(buf, BUFFER_SIZE);
	    size = nrecv(&http_client->connection, buf, BUFFER_SIZE);
	    if(size > 0) {
		timeout = 3;
	        memcpy(response->enti_body.buffer + recv_body_length, buf, size);
	        recv_body_length += size;
	    } else { 
		switch(size) {
		case RECV_OVER:
		    return RESPONSE_OK;
		break;
		case RECV_TIMEOUT:
		    timeout--;
		    if(!timeout) return RESPONSE_TIMEOUT;
		break;
		case RECV_ERROR:
		    return RESPONSE_FAILED;
		break;
		}
	    }
	    break;
	default:
	    fprintf(stderr, "Unknown context type in func 'do_get'.\n");
	}
    } while(1); 
    
    return size < 0 ? RESPONSE_FAILED : RESPONSE_OK;
}

int http_response_status(http_client_t *http_client)
{ 
    return atoi(http_client->response.res_line.status_code);
}
const char * http_response_body(http_client_t *http_client)
{
    return http_client->response.enti_body.buffer;
}

int http_response_body_length(http_client_t *http_client)
{
    return http_client->response.enti_body.len;
}



