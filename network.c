/*****
** network.c
** - implements the methods declared in network.h
** - 对网络socket的收发进行封装
*/
#include "network.h"

int open_tcp(network_t *network, char *hostname, int port) 
{
    int clientfd, ret, res;
    char buffer[1024];
    struct hostent *hp, hostbuf;
    struct sockaddr_in serveraddr;

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return SOCKET_FAILED;
    }
    /* setting recv timeout 10s*/
    struct timeval timeout = {10, 0};  
    setsockopt(clientfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
    /* check errno for cause of error */
    /* Fill in the server's IP address and port */
    res = gethostbyname_r(hostname, &hostbuf, buffer, sizeof(buffer), &hp, &ret);
    if(res || hp == NULL) {
        return HOST_FAILED; /* check h_errno for cause of error */
    }
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);
    /* Establish a connection with the server */
    if (connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        return CONNECT_FAILED;
    }
    network->socketfd = clientfd;
    network->len = 0;
    strcpy(network->host, hostname);
    network->port = port;
    bzero(network->buffer, BUFFER_SIZE);
 
    return CONNECT_OK;
}

int nsend(network_t *network, char *buf, int bytes)
{
    return send(network->socketfd, buf, bytes, 0);
}

int nrecv(network_t *network, char *buf, int bytes)
{
    int offset, rest, size = 0, recv_size;
    offset = bytes <= network->len ? bytes : network->len;
    rest = network->len - offset;
    memcpy(buf, network->buffer, offset);
/*
    strncat(buf, network->buffer, offset); */
    memmove(network->buffer, network->buffer + offset, rest); 
    network->buffer[rest] = '\0';
    network->len = rest;
    size += offset;
    if(rest > 0) { return size; }

    recv_size = recv(network->socketfd, buf + offset, bytes - offset, 0);
	
    /* if recv failed, but copy buffer size success need return */
    size += recv_size >= 0 ? recv_size : 0;
    /* if recv size > 0, then size > 0, so return size */
    return size > 0 ? size : 
	/* if size == 0 may be recv failed or recv over, but if recv_size == 0 then recv over*/
		(recv_size == 0 ? recv_size : 	
	/*but recv_size < 0, we need check timeout or recv_error*/
			(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN ? RECV_TIMEOUT : RECV_ERROR));
}

int recv_line(network_t *network, char *buf, int limit)
{
    char *p_delm = NULL;
    /*  size: a line total bytes
	b_size: network->buffer now contain character
	rest: length belongs to next line but recv in this nrecv
	offset: first \n pointer in this recv
    */
    int size = 0, b_size = network->len, rest, offset;
    
    bzero(buf, limit);
    do {
	/* copy network->buffer data to buf, if a line enough return */
    	p_delm = strchr(network->buffer, '\n');
	offset = p_delm != NULL ? p_delm - network->buffer + 1 : network->len;
	rest = b_size - offset;
	if(size > limit) { return -1; }
	memcpy(buf + size, network->buffer, offset);
	/*strncat(buf, network->buffer, offset); */
	if(p_delm != NULL) { 
	    memmove(network->buffer, p_delm + 1, rest); 
	    network->buffer[rest] = '\0';
	}
	network->len = rest;
	size += offset;
	if(rest > 0) { return size; }
	bzero(network->buffer, BUFFER_SIZE);
	/* if a line not enough, go on read */
        b_size = nrecv(network, network->buffer, BUFFER_SIZE);
    } while(b_size > 0);

    return b_size < 0 ? b_size : size;
}

void close_tcp(network_t *network)
{
    close(network->socketfd);
}
