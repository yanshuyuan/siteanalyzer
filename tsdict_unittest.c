#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include "ts_dict.h"
#include "hash.h"

ts_dict_t ts_dict;

void thread_func(void *arg) {
    int pid = *(int *)arg;
    sleep(1);
    char buffer[32] = {};
    sprintf(buffer, "thread:%d ", pid);
    ts_dict_insert(&ts_dict, buffer, strlen(buffer));
    ts_dict_remove(&ts_dict, buffer, strlen(buffer));
    ts_dict_find(&ts_dict, buffer, strlen(buffer));
} 


int main()
{
    ts_dict_init(&ts_dict, 2, sdbm_hash);
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
    

    ts_dict_destroy(&ts_dict);


}

