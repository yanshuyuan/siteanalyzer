/*****
** crawler.c
** - implements the methods declared in crawler.h
** -爬取线程的核心流程
*/
#include "link_crawler.h"

int crawler_crawl(link_crawler_t *crawler, char *url, list_t *link_list)
{
    http_url_t http_url;
    int status = CRAWLER_NULL;
    if(http_url_parse_s(&http_url, url) == URL_RECOGNIZED) {
	int port = strlen(http_url.port) != 0 ? atoi(http_url.port) : 80;
	char path[4096] = {""};
	strlen(http_url.search) == 0 ? sprintf(path, "%s", http_url.path) : sprintf(path, "%s?%s", http_url.path, http_url.search);

        if(http_connect(&crawler->http_client, http_url.host, port) == CONNECT_OK) {
            int ecode, response_status; 
            const char *page = NULL;
	    printf("%s\n", url);
            ecode = http_do_get(&crawler->http_client, path);
            switch(ecode) {
            case RESPONSE_OK:
                response_status = http_response_status(&crawler->http_client);
                if(response_status == HTTP_OK) {
                    /* get entity_body pointer */
                    page = http_response_body(&crawler->http_client);
                    /* extrack link from buffer and save link into list*/
		    if(page != NULL) {
                        extract_absolute_link_s(page, link_list, url);
            	        status = CRAWLER_OK;
		    } else {
			status = CRAWLER_NULL;
                        fprintf(stderr, "Request %s:%d%s failed, response body is null.\n",
                		crawler->http_client.connection.host, 
                		crawler->http_client.connection.port, path);
		    }
                } else {
                    fprintf(stderr, "Request %s:%d%s failed, Status code: %d.\n", 
                		crawler->http_client.connection.host, 
                		crawler->http_client.connection.port, path, response_status);
                    status = CRAWLER_NONEED;
                }
                break;
            case RESPONSE_OVERFLOW:
                fprintf(stderr, "Request %s:%d%s do_get receive overflow.\n", 
                		crawler->http_client.connection.host, 
                		crawler->http_client.connection.port, path);
                status = CRAWLER_OVERFLOW;
		break;
            case RESPONSE_FAILED:
                fprintf(stderr, "Request %s:%d%s do_get receive break.\n", 
                		crawler->http_client.connection.host, 
                		crawler->http_client.connection.port, path);
                status = CRAWLER_BREAK;
                break;
	    case RESPONSE_TIMEOUT:
		fprintf(stderr, "Request %s:%d%s do_get receive timeout.\n", 
                		crawler->http_client.connection.host, 
                		crawler->http_client.connection.port, path);
                status = CRAWLER_TIMEOUT;
		break;
            case REQUEST_FAILED:
                fprintf(stderr, "Request %s:%d%s do_get request failed.\n", 
                		http_url.host, port, path);
                status = CRAWLER_FAILED;
                break;
            default:
                fprintf(stderr, "Unknown ecode %d.\n", ecode);
		status = CRAWLER_UNKNOWN;
                break;
            }
	    http_disconnect(&crawler->http_client);
        } else {
            fprintf(stderr, "Http connect %s:%d failed.\n", http_url.host, port);
            status = CRAWLER_UNREACH;
        }
    } else {
	fprintf(stderr, "Unrecognize url: %s\n", url);
	status = CRAWLER_UNKNOWN; 
    }
    return status;
}

