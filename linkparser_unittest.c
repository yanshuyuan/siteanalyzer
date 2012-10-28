#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "link_parser.h"
#include "list.h"

void printList(list_t *l) {
    elem_t *iter;
    int i;
    printf("---------------------------------------------------------------\n");
    for(iter = list_first(l); iter != list_end(l); iter = list_next(iter)) {
	printf("Link: ");
	for(i = 0; i < iter->len; i++) {
	    putchar(iter->buffer[i]);
        }
        putchar('\n');
    }
    printf("---------------------------------------------------------------\n");
    list_clear(l);

}

int main() {
    char html[1024];
    list_t urllist; 
    list_init(&urllist);
    /* test for legal pattern */
    printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    char pattern1[] = {"<div><a class=\"%s\" href=\"%s\" version=\"%s\">link</a><div class=\"mblog_login_text\" id=\"mblog_login_text\"><a href=\"http://t.qq.com/reg/index.php?pref=qqcom.mininav\" target=\"_blank\">ע��</a></div><div class=\"mblog_loginout_text\" id=\"mblog_logout_text\"><a href=\"javascript:void(0)\" target=\"_blank\">�˳�</a></div>\n"
	"<div class=\"sub\"><a href=\"http://rss.qq.com/tech.htm\" class=\"rss\" target=\"_blank\">RSS</a><a>link</a></div><a href=\"www.baidu.com\">\n"
		"Link<a>Likn\n"
		"</a>\n\n"};
    sprintf(html, pattern1, "button", "www.baidu.com?uid=a3ve17g2j5w", "1");
    extract_absolute_link(html, &urllist, "www.baidu.com?uid=a3ve17g2j5w");
    printList(&urllist);
    printf("------------------s---------------------------\n");
    extract_absolute_link_s(html, &urllist, "www.baidu.com?uid=a3ve17g2j5w");
    printList(&urllist);
    printf("--------------------------------------<><><><><>\n");
    /* test for illegal pattern */

    char pattern2[] = {"<div><a class=\"%s\" href=\"%s\" version=\"%s\">link</a><div class=\"mblog_login_text\" id=\"mblog_login_text\"><a href=\"http://t.qq.com/reg/index.php?pref=qqcom.mininav\" target=\"_blank\">ע��</a></div><div class=\"mblog_loginout_text\" id=\"mblog_logout_text\"><a href=\"javascript:void(0)\" target=\"_blank\">�˳�</a></div>\n"
	"<div class=\"sub\"><a href=\"./../tech.htm\" class=\"rss\" target=\"_blank\">RSS</a><a href='#'>link</a></div><a ha href  =  \"../../../index.html\"  >\n"
		"Link<a\n>Likn\n"
		"Link<a href='baidu.com'\n"
		"Link<a href = ''>Likn\n"
		"</a>\n\n<a href=''>"};

    sprintf(html, pattern2, "button", "00001.html", "1");
    extract_absolute_link(html, &urllist, "http://125.211.218.8/techqq/a/20121008/000002.htm");
    printList(&urllist);
    printf("------------------s---------------------------\n");
    extract_absolute_link_s(html, &urllist, "http://125.211.218.8/techqq/a/20121008/000002.htm");
    printList(&urllist);
    
    list_destroy(&urllist);

    return 0;
}
	
