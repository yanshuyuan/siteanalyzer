#include <stdio.h>
#include <stdlib.h>

#include "http_client.h"

int main()
{

    const char *body_ptr;
    int status;
    http_client_t http_client1;
    http_connect(&http_client1, "125.211.218.8", 80);
    http_do_get(&http_client1, "/techqq/index.html");
    status = http_response_status(&http_client1);
    body_ptr = http_response_body(&http_client1);
    printf("---------------------Status-----------------------\n");
    printf("%d\n", status);
    printf("---------------------Body-----------------------\n");
    printf("%s\n", body_ptr);
    http_disconnect(&http_client1);


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

}
