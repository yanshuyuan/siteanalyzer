#include <stdio.h>
#include <stdlib.h>

#include "http_client.h"

int main(int argc, char **argv)
{
   char *host = "10.205.42.139";
   char *path = "/techqq/a/20090423/000378.htm";
   /*
    char *host = "125.211.218.8";
char *path = "/techqq/a/20120111/000508.htm";
char *path = "/techqq/a/20121008/000013.htm";
    char *path = "/techqq/wlyx.htm";
    char *path = "/techqq/index.html";
    char *path = "/techqq/a/20121008/000048.htm";
10.205.42.139:80/techqq/a/20090423/000378.htm
*/
    const char *body_ptr;
    int status;
    http_client_t http_client1;
    http_connect(&http_client1, host, 80);
    http_do_get(&http_client1, path);
    status = http_response_status(&http_client1);
    body_ptr = http_response_body(&http_client1);
    printf("---------------------Status-----------------------\n");
    printf("%d\n", status);
    printf("---------------------Body-----------------------\n");
    printf("%p\n", body_ptr);
    printf("%s\n", body_ptr);
    http_disconnect(&http_client1);

/*
    status = http_response_status(&http_client1);
    body_ptr = http_response_body(&http_client1);
*/
/*
    printf("---------------------Status-----------------------\n");
    printf("%d\n", status);
    printf("---------------------Body-----------------------\n");
    printf("%s\n", body_ptr);
*/
/*


    http_client_t http_client2;
    http_connect(&http_client2, "ufp.umeng.com", 80);
    http_do_get(&http_client2, "/login");

    status = http_response_status(&http_client2);
    body_ptr = http_response_body(&http_client2);

    printf("---------------------Status-----------------------\n");
    printf("%d\n", status);
    printf("---------------------Body-----------------------\n");
    printf("%s\n", body_ptr);
    http_disconnect(&http_client2);


    http_client_t http_client3;
    http_connect(&http_client3, "blog.umeng.com", 80);
    http_do_get(&http_client3, "/index.php/category/products/");

    status = http_response_status(&http_client3);
    body_ptr = http_response_body(&http_client3);

    printf("---------------------Status-----------------------\n");
    printf("%d\n", status);
    printf("---------------------Body-----------------------\n");
    printf("%s\n", body_ptr);
    http_disconnect(&http_client3);
*/

}
