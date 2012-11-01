#include "http_url.h"

int _check_protocol(char *protocol, int len)
{
    int i;
    for(i = 0; i < len; i++) {
	if(!isalnum(protocol[i])) {
	    return 0;
	}
    }
    return 1;
}

int _check_host(char *host, int len)
{
    int i;
    for(i = 0; i < len; i++) {
	if(!(isalnum(host[i]) || host[i] == '.' || host[i] == '-')) {
	    return 0;
	}
    }
    return 1;
}

int _check_port(char *port, int len)
{
    int i;
    for(i = 0; i < len; i++) {
	if(!isdigit(port[i])) {
	    return 0;
	}
    }
    return 1;

}

int _check_path(char *path, int len)
{
    int i;
    for(i = 0; i < len; i++) {
	if(!(isalnum(path[i]) || path[i] == '/' || path[i] == ';'
		|| path[i] == ':' || path[i] == '@' || path[i] == '%'
		|| path[i] == '&' || path[i] == '='
		|| path[i] == '_' || path[i] == '.'
		|| path[i] == '-' || path[i] == '+')) {
	    return 0;
	}
    }
    return 1;
}

int http_url_parse_s(http_url_t *http_url, char *url) 
{
    bzero(http_url, sizeof(http_url_t));
    char *str, *cstr = url;
    char *estr = url + strlen(url);
    str  = strstr(url, "://");
    /* protocol */
    if(str != NULL) {
	if(_check_protocol(cstr, str - cstr)) {
            memcpy(http_url->protocol, cstr, str - cstr);
    	    cstr = str + 3;
	} else {
	    return URL_PROTO_UNRECOGNIZED;
	}
    }
    str = strchr(cstr, ':');
    if(str != NULL) {
	if( _check_host(cstr, str - cstr)) { /* contain port */
	    memcpy(http_url->host, cstr, str - cstr);
	    cstr = str + 1;
	    str = strchr(cstr, '/'); /* copy port */
	    str = str != NULL ? str : estr;
	    if(_check_port(cstr, str - cstr)) {
	        memcpy(http_url->port, cstr, str - cstr);
	    } else {
	        return URL_PORT_UNRECOGNIZED;
	    }
	    cstr = str;
	} else {
	    return URL_HOST_UNRECOGNIZED;
	}
    } else { /* no port */
	str = strchr(cstr, '/'); /* copy port */
	str = str != NULL ? str : estr;
	if(_check_host(cstr, str - cstr)) {
	    memcpy(http_url->host, cstr, str - cstr); /* copy host */
	    cstr = str;
	} else {
	    return URL_HOST_UNRECOGNIZED;
	}
    }
    /* copy path */
    str = strchr(cstr, '?');
    if(str != NULL) { /* contain search */
	if(_check_path(cstr, str - cstr)) {
            memcpy(http_url->path, cstr, str - cstr);
            cstr = str + 1;
	    strcpy(http_url->search, cstr);
	} else {
	    return URL_PATH_UNRECOGNIZED;
	}
    } else { /* not contain search */
	if(_check_path(cstr, strlen(cstr))) {
	    strcpy(http_url->path, cstr);
	} else {
	    return URL_PATH_UNRECOGNIZED;
	}
    }
    return URL_RECOGNIZED;
}

int http_url_parse(http_url_t *http_url, char *url) 
{
#ifndef __HTTP_URL_PADDING
#define __HTTP_URL_PADDING(_dest, _src, _so, _eo) { \
    bzero(_dest, sizeof(_dest));\
    if(_so < _eo) { memcpy(_dest, _src + _so, _eo - _so); } \
} 
#endif
    const size_t nmatch = 12;
    int status = URL_UNRECOGNIZED, cflag, ecode;
    regex_t reg; 
    char ebuf[128];
    regmatch_t pmatch[nmatch];
    cflag = REG_NEWLINE | REG_EXTENDED;
    bzero(http_url, sizeof(http_url_t));
    const char *pattern = __HTTPURL;
/*
    __HTTPURL below:
   ^((http[s]?)://)?([a-zA-Z0-9][-a-zA-Z0-9_]{0,62}(\.[a-zA-Z0-9][-a-zA-Z0-9_]{0,62})+\.?)(:[0-9]+)?((/[a-zA-Z0-9;:@&=_.]*(/[a-zA-Z0-9;:@&=_.]*)*)(\?([-a-zA-Z0-9;:@&=,]*))?)?$ 
*/
	
    if((ecode = regcomp(&reg, pattern, cflag)) == 0) {
        if(regexec(&reg, url, nmatch, pmatch, 0) != REG_NOMATCH) {
	    status = URL_RECOGNIZED;
	    __HTTP_URL_PADDING(http_url->protocol, url, pmatch[2].rm_so, pmatch[2].rm_eo);
	    __HTTP_URL_PADDING(http_url->host, url, pmatch[3].rm_so, pmatch[3].rm_eo);
	    __HTTP_URL_PADDING(http_url->port, url, pmatch[6].rm_so, pmatch[6].rm_eo);
	    __HTTP_URL_PADDING(http_url->path, url, pmatch[8].rm_so, pmatch[8].rm_eo);
	    __HTTP_URL_PADDING(http_url->search, url, pmatch[11].rm_so, pmatch[11].rm_eo);
        }
    } else {
	regerror(ecode, &reg, ebuf, sizeof(ebuf));
	fprintf(stderr, "%s: url '%s' parse failed.\n", ebuf, url);
    }
    regfree(&reg);
    return status;

}

