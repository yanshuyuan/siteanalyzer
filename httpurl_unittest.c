#include "http_url.h"
#include <stdio.h>
#include <stdlib.h>

void printHttpUrl(http_url_t *http_url, char *url)
{
    printf("----------------------------\n");
    http_url_parse_s(http_url, url);
    printf("origin: %s\n", url);
    printf("protocol: %s\n", http_url->protocol);
    printf("host: %s\n", http_url->host);
    printf("port: %s\n", http_url->port);
    printf("path: %s\n", http_url->path);
    printf("search: %s\n", http_url->search);
    printf("----------------------------\n\n");
}
int main()
{
/*
    printf("digit: %s\n",  __DIGIT);
    printf("character: %s\n",  __CHAR);
    printf("alpha: %s\n",  __ALPHA);
    printf("search: %s\n",  __SEARCH);
    printf("segment: %s\n",  __HSEGMENT);
    printf("path: %s\n",  __HPATH);
    printf("host: %s\n",  __HOST);
    printf("port: %s\n",  __PORT);
    printf("hostport: %s\n",  __HOSTPORT);
    printf("httpurl: %s\n",  __HTTPURL);

*/
    http_url_t http_url;

    char *url1 = "http://blog.tian-ya.cn/blogger/post_show.asp?BlogID=2014653&PostID=17376058";
    printHttpUrl(&http_url, url1);

    char *url2 = "http://www.cnblogs.com/wlly216/archive/2011/10/01/2197181.html";
    printHttpUrl(&http_url, url2);

    char *url3 = "http://www.css88.com/archives/3643";
    printHttpUrl(&http_url, url3);

    char *url4 = "http://www.google.com.hk/search?aq=f&sugexp=chrome,mod=12&sourceid=chrome&ie=UTF-8&q=rfc1945";
    printHttpUrl(&http_url, url4);

    char *url5 = "archives/3643";
    printHttpUrl(&http_url, url5);

    char *url6 = "http://t.qq.com/?pref=qqcom.toptips";
    printHttpUrl(&http_url, url6);

    char *url7 = "javascript:void(0)";
    printHttpUrl(&http_url, url7);

    char *url8 = "index.html";
    printHttpUrl(&http_url, url8);
	
    char *url9 = "http://c.t.qq.com/i/883?filter=1&amp;pref=qqcom.tech.nav.text";
    printHttpUrl(&http_url, url9);

    char *url10 = "http://news.qq.com/mobile/";
    printHttpUrl(&http_url, url10);

    char *url11 = "https://125.211.218.8:8080/techqq/a/20121004/000062.htm";
    printHttpUrl(&http_url, url11);

    char *url12 = "www.baidu.com";
    printHttpUrl(&http_url, url12);

    printf("----------------------------\n");


    return 0;
}

