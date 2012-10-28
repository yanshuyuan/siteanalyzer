#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

#define CONNECT_OK 0
#define SOCKET_FAILED 1
#define HOST_FAILED 2
#define CONNECT_FAILED 3
#define RECV_TIMEOUT -1
#define RECV_ERROR -2
#define RECV_OVER 0 


typedef struct _network {
    int socketfd;
    char host[64];
    int port;
    int len; /*buffer current len*/
    char buffer[BUFFER_SIZE]; /*recv line buffer*/
} network_t;

int open_tcp(network_t *network, char *hostname, int port);

int nsend(network_t *network, char *buf, int bytes);

int nrecv(network_t *network, char *buf, int bytes);

void close_tcp(network_t *network);

int recv_line(network_t *network, char *buf, int limit);


#endif
