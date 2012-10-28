#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include "ts_queue.h"

ts_queue_t ts_queue;

void thread_func(void *arg) {
    int pid = *(int *)arg;
    sleep(pid % 3 + 1);
    char buffer[32];
    bzero(buffer, 32);
    sprintf(buffer, "thread:%d ", pid);
    if(pid % 3 == 0) { 
	printf("%s push queue, element: %s\n", buffer, buffer);
	ts_queue_push(&ts_queue, buffer, strlen(buffer));
    } else if(pid % 3 == 1) {
	printf("%s front queue\n", buffer);
	char *s = ts_queue_front(&ts_queue);
	if(s != NULL) {
	    printf("front element: %s\n", s);
	}
    } else {
	printf("%s top queue\n", buffer);
	char str[32];
	ts_queue_top(&ts_queue, str);
	printf("top element: %s\n", str);
    }
} 


int main()
{
    ts_queue_init(&ts_queue);
    pthread_t c_thread[10];
    void *retval[10];
    int param[10];
    int i;
    for(i = 0; i < 10; i++) {
	param[i] = i;
	pthread_create(&c_thread[i], NULL, thread_func, (void *)&param[i]);
    }
    for(i = 0; i < 10; i++) {
	pthread_join(c_thread[i], NULL);
    }
    

    ts_queue_destroy(&ts_queue);


}

