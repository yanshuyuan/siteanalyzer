/*****
** http_client.c
** - implements the methods declared in http_client.h
** - 封装处理http协议
*/

#include "http_client.h"
#include "assert.h"

#define REQ_SIZE 4096 
#define BUFFER_SIZE 8192
#define LIMIT 4096

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
    bzero(http_request, REQ_SIZE);
    int size; /* send or recv timeout more than 3 times will stop */
	
    sprintf(http_request, "GET %s HTTP/1.0\r\n"
			"Host: %s\r\n"
			"User-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.11 "
				"(KH| TML, like Gecko) Ubuntu/11.10 Chromium/17.0.963.79 Chrome/17.0.963.79 Saf "
				"| ari/535.11\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n\r\n", path, http_client->connection.host);

    size = nsend(&http_client->connection, http_request, strlen(http_request));

    if(size < 0) {
	fprintf(stderr, "Http Request failed, error code: %d\n", size);
	return REQUEST_FAILED;
    }

    char buf[BUFFER_SIZE];
    http_res_t *response = &http_client->response;
   
    int entity_body_size, recv_body_length = 0;
    entity_body_size = ENTITY_BODY_SIZE_DEFAULT;
    
    /* recv http response */
    response->enti_body.buffer = (char *) malloc(ENTITY_BODY_SIZE_DEFAULT);
    if(response->enti_body.buffer == NULL) {
        response->enti_body.len = 0;
        fprintf(stderr, "Error: allocate memory failed in func 'do_get'.\n");
        return RESPONSE_FAILED;
    } else {
        response->enti_body.len = entity_body_size;
        bzero(response->enti_body.buffer, entity_body_size);

        do {
            bzero(buf, BUFFER_SIZE);
            size = nrecv(&http_client->connection, buf, LIMIT);
            if(size > 0) {
                if(recv_body_length + size > entity_body_size) {
		    entity_body_size *= 2;
		    response->enti_body.buffer = (char *)realloc(response->enti_body.buffer, entity_body_size);
		    if(response->enti_body.buffer == NULL) {
			response->enti_body.len = 0;
                        return RESPONSE_FAILED;
		    } else {
			response->enti_body.len =entity_body_size;
		    }
		
                }
                memcpy(response->enti_body.buffer + recv_body_length, buf, size);
                recv_body_length += size;
            } else if(size == 0) {
		response->enti_body.buffer[recv_body_length] = '\0';
		return RESPONSE_OK;
            }else {
		return RESPONSE_FAILED;
            }
        } while(1); 
    }
    
    return RESPONSE_FAILED;
}

int http_response_status(http_client_t *http_client)
{ 
    char buffer[4096];
    int status_code = 0;
    http_res_t *response = &http_client->response;
    sscanf(response->enti_body.buffer, "%s%d", buffer, &status_code);
    printf("status code: %d\n", status_code);
    return status_code;
    /*
    char *s = strchr(response->enti_body.buffer, '\n');
    if(s != NULL) {
        memcpy(buffer, response->enti_body.buffer, s - response->enti_body.buffer + 1);  
        printf("%s\n", buffer);
        sscanf(buffer, "%s %s %s", response->res_line.http_version, response->res_line.status_code, response->res_line.reason_phrase);
        return atoi(http_client->response.res_line.status_code);
    } else {
	return 0;
    }
    */
}

const char * http_response_body(http_client_t *http_client)
{
    return http_client->response.enti_body.buffer;
}




