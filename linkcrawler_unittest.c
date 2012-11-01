#include <stdio.h>
#include <stdlib.h>
#include "link_crawler.h"

void printList(list_t *l) {
    elem_t *iter;
    int i;
    printf("---------------------------------------------------------------\n");
    for(iter = list_first(l); iter != list_end(l); iter = list_next(iter)) {
	printf("Link: %s\n", iter->buffer);
	/*
	for(i = 0; i < iter->len; i++) {
	    putchar(iter->buffer[i]);
        }
        putchar('\n');
	*/
    }
    printf("---------------------------------------------------------------\n");
    list_clear(l);

}

int main(int argc, char **argv)
{
    list_t l;
    list_init(&l);
	/*
/techqq/a/20120618/000047.htm
    char *url1 = "http://125.211.218.8/techqq/a/20120626/000158.htm";
    char *url1 = "http://125.211.218.8/techqq/a/20060224/1.html";
    char *url1 = "http://125.211.218.8/techqq/a/20110926/000359.htm";
    char *url1 = "http://125.211.218.8/techqq/a/20101109/000034.htm";
    char *url1 = "http://125.211.218.8/techqq/a/20121007/000028.htm";
	*/
    char *url1 = argv[1];
    link_crawler_t crawler1;
    crawler_crawl(&crawler1, url1, &l);
    printList(&l);

/*
    char *url2 = "http://topic.csdn.net/u/20100108/13/a82399e8-78b5-404f-b22c-7f5b2731ed30.html"; 
    link_crawler_t crawler2;
    crawler_crawl(&crawler2, url2, &l);
    printf("-=============================\n");
    printList(&l);
  	
    char *url3 = "http://125.211.218.8:80/techqq/a/20121007/000001.htm";
    link_crawler_t crawler3;
    crawler_crawl(&crawler3, url3, &l);
    printList(&l);

    char *url4 = "http://125.211.218.8/techqq/a/20110308/000158.htm";
    link_crawler_t crawler4;
    crawler_crawl(&crawler4, url4, &l);
    printList(&l);
           	
    char *url5 = "http://ufp.umeng.com/login";
    link_crawler_t crawler5;
    crawler_crawl(&crawler5, url5, &l);
    printList(&l);
           	
    char *url6 = "http://125.211.218.8/techqq/a/20120315/000066.htm";
    link_crawler_t crawler6;
    crawler_crawl(&crawler6, url6, &l);
    printList(&l);
    char *url7 = "http://125.211.218.8/techqq/index.html";
    link_crawler_t crawler7;
    crawler_crawl(&crawler7, url7, &l);
    printList(&l);

    char *url8 = "http://125.211.218.8/techqq/a/20100125/000227.htm";
    link_crawler_t crawler8;
    crawler_crawl(&crawler8, url8, &l);
    printList(&l);
*/
}
