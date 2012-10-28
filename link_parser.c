/*****
** link_parser.c
** - implements the methods declared in link_parser.h
** - 处理http响应字节流，提取网络链接
*/

#include <stdio.h>
#include <regex.h>
#include <string.h>
#include "link_parser.h"
#include "http_url.h"
#define BUFFER_SIZE 1024

/* return then root len of url */
int _search_root(const char *link, size_t offset, char *url, link_offset_t *link_offset)
{
    int c = 0, layer = 1, flag = 1;
    if(offset >= 7 && memcmp(link, "http://", 7) == 0) {
	return 1;
    } else {
        while(flag) {
            if(c + 2 < offset && memcmp(link + c, "./", 2) == 0) {
                c += 2;
            } else if(c + 3 < offset && memcmp(link + c, "../", 3) == 0) {
                c += 3;
                layer++;
            } else {
                flag = 0;
            }
        }
	int i, len = strlen(url);
        for(i = len - 1; i >= 0; i--) {
            if(url[i] == '/') { layer--; }
	    if(layer == 0) { break; }
        }
	link_offset->path_so = c;
	link_offset->path_eo = offset;
	link_offset->pre_so = 0;
	link_offset->pre_eo = i + 1;

        return 0;
    }
    
}

void extract_absolute_link_s(const char *htmltxt, list_t *urllist, char *url)
{
    enum {S_START, S_LEFT, S_A, S_BLANK, S_H, S_R, S_E, S_F, S_EQ, S_LQ, S_RQ, S_RIGHT, S_END};
    const char *ch = htmltxt;
    int status = S_START;
    const char *as = NULL,  *ae = NULL, *ls = NULL, *le = NULL;
    link_offset_t link_offset;
    char link[4096];
    int prefix_size, path_size, link_size;
    int k = 0;

    while(*ch) {
	switch(status) {
	case S_START:
	    if(*ch == '<') {
		status = S_LEFT;
		as = ch;
	    } else if(*ch == '\n') {
		status = S_END;
	    }
	    k++;
	    ch++;
	break;
	case S_LEFT:
	    if(*ch == 'a') {
		status = S_A;
	    } else if(*ch == '>') {
		ae = ch;
		status = S_RIGHT;
		continue;
	    } else if(!(*ch == '\t' || *ch == ' ')) {
		status = S_END;
	    }
	    k++;
	    ch++;
	break;
	case S_A:
	    if(*ch == '\t' || *ch == ' ') {
		status = S_BLANK;
	    } else if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else {
		status = S_END;
	    }
	    k++;
	    ch++;
	break;
	case S_BLANK:
	    if(*ch == '>') {
	        status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
	        status = S_END;
	    } else if(*ch == 'h') {
	        status = S_H;
	    }
	    k++;
	    ch++;
	break;
	case S_H:
	    if(*ch == '>') {
	        status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
	         status = S_END;
	    } else if(*ch == 'r') {
	         status = S_R;
	    } else {
	         status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_R:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == 'e') {
		status = S_E;
	    } else {
		status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_E:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == 'f') {
		status = S_F;
	    } else {
		status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_F:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == '=') {
		status = S_EQ;
	    } else if(!(*ch == '\t' || *ch == ' ')) {
		status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_EQ:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == '"' || *ch == '\'') {
		ls = ch + 1;
		status = S_LQ;
	    } else if(!(*ch == '\t' || *ch == ' ')) {
		status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_LQ:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == '"' || *ch == '\'') {
		le = ch - 1 ;
		status = S_RQ;
	    }
	    k++;
	    ch++;
	break;
	case S_RQ:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    }
	    k++;
	    ch++;
	break;
	case S_RIGHT:
	    status = S_START;
	    if(!(ls == NULL || le == NULL)) {
		bzero(link, 4096);
		link_size = le - ls + 1;
	        if(!_search_root(ls, link_size, url, &link_offset)) {
	            prefix_size = link_offset.pre_eo - link_offset.pre_so; /* prefix_size */
	            path_size = link_offset.path_eo - link_offset.path_so; /* path_size */
		    if(prefix_size > 0 && path_size > 0) {
	                memcpy(link, url + link_offset.pre_so, prefix_size); /* copy prefix */
	                memcpy(link + prefix_size, ls + link_offset.path_so, path_size); /* copy path */
	                list_push_back(urllist, link, prefix_size + path_size + 1);
		    }
                } else {
		    if(link_size > 0) {
	                memcpy(link, ls, link_size);
	                list_push_back(urllist, link, link_size + 1);
		    }
	        }
	    }
	/*
	    printf("===============<a>======================\n");
	    printf("url: \n");
	    if(ls != NULL && le != NULL) {
	        for(i = 0; i <= le - ls; i++) {
		    printf("%c", ls[i]);
	        }
	        putchar('\n');
	    }
	    printf("=====================================\n");
	*/
	    as = ae = ls = le = NULL;
	    k++;
	    ch++;
	break;
	case S_END:
	    status = S_START;
	    as = ae = ls = le = NULL;
	break;
	}
    }
}

/*
     the extract_link will change the info of the htmltxt,
	if you will call it and stay the content of htmltxt not change,
	 you should pass the copy of htmltxt
*/



void extract_absolute_link(const char *htmltxt, list_t *urllist, char *url)
{
    const size_t nmatch = 2;
    int cflag, ecode;
    char ebuf[128];
    regex_t reg;
    regmatch_t pmatch[nmatch];
    cflag = REG_NEWLINE | REG_EXTENDED;
    const char *ptr = htmltxt, *link_ptr;
    link_offset_t link_offset; 
    int link_size, prefix_size, path_size;
    char link[4096];

    const char *pattern = "<[[:blank:]]*a[^>]*href[[:blank:]]*=[[:blank:]]*['\"]([^'\"]*)['\"][^>]*>";
    if((ecode = regcomp(&reg, pattern, cflag)) == 0) {
	/* get a <a ...>(...</a>)? in a line 	*/
        ptr = htmltxt; 
	while(regexec(&reg, ptr, nmatch, pmatch, 0) != REG_NOMATCH) {
	    link_ptr = ptr + pmatch[1].rm_so;
	    link_size = pmatch[1].rm_eo - pmatch[1].rm_so;
	    bzero(link, 4096);
	    if(!_search_root(link_ptr, link_size, url, &link_offset)) {
		prefix_size = link_offset.pre_eo - link_offset.pre_so; /* prefix_size */
		path_size = link_offset.path_eo - link_offset.path_so; /* path_size */
		memcpy(link, url + link_offset.pre_so, prefix_size); /* copy prefix */
		memcpy(link + prefix_size, link_ptr + link_offset.path_so, path_size); /* copy path */
		list_push_back(urllist, link, prefix_size + path_size + 1);
            } else {
	        memcpy(link, link_ptr, link_size);
	    	list_push_back(urllist, link, link_size + 1);
	    }
	    ptr = ptr + pmatch[0].rm_eo + 1;
        }
    } else {
	regerror(ecode, &reg, ebuf, sizeof(ebuf));
	fprintf(stderr, "%s: pattern '%s' complie failed.\n", ebuf, pattern);
    }
    regfree(&reg);


}

void extract_link(const char *htmltxt, list_t *urllist) {
    const size_t nmatch = 2;
    int cflag, ecode;
    char ebuf[128];
    char link[4096];
    regex_t reg;
    regmatch_t pmatch[nmatch];
    cflag = REG_NEWLINE | REG_EXTENDED;
    const char *ptr = htmltxt;

    const char *pattern = "<[[:blank:]]*a[^>]*href[[:blank:]]*=[[:blank:]]*['\"]([^'\"]*)['\"][^>]*>";
    if((ecode = regcomp(&reg, pattern, cflag)) == 0) {
	/* get a <a ...>(...</a>)? in a line 	*/
        ptr = htmltxt; 
	while(regexec(&reg, ptr, nmatch, pmatch, 0) != REG_NOMATCH) {
	    bzero(link, 4096);
	    memcpy(link, ptr + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
	    list_push_back(urllist, link, pmatch[1].rm_eo - pmatch[1].rm_so + 1);
	    ptr = ptr + pmatch[0].rm_eo;
        }
    } else {
	regerror(ecode, &reg, ebuf, sizeof(ebuf));
	fprintf(stderr, "%s: pattern '%s' complie failed.\n", ebuf, pattern);
    }
    regfree(&reg);
}
