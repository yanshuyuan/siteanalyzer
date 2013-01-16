/*****
** main.c
** - 主线程，维持线程池，线程间通信等
*/
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <semaphore.h>
#include "thread_pool.h"
#include "thread.h"
#include "queue.h"
#include "ts_queue.h"
#include "worker.h"
#include "webgraph.h"
#include "http_url.h"

#define MAX_THREADS 300


thread_pool_t thread_pool;
thread_counter_t global_lock;
ts_queue_t urlqueue;
sem_t work_sem;
webg_t webgraph;
char domain[1024];

int main(int argc, char **argv)
{
    if(argc < 3) {
	fprintf(stderr, "too few argument, 3 arguments expected.\n");
	return 1;
    }
    char *url = argv[1];
    char *path = argv[2];
    http_url_t http_url;
    http_url_parse_s(&http_url, url);
    if(strlen(http_url.protocol) != 0) sprintf(domain, "%s://", http_url.protocol);
    strcat(domain, http_url.host);

    init_webg(&webgraph);
    sem_init(&work_sem, 0, 0);
    ts_queue_init(&urlqueue);

    thread_pool_init(&thread_pool, MAX_THREADS);
    thread_pool_run(&thread_pool);
	
/*
    char *url = "http://125.211.218.8/techqq/index.html"; 
    url = "http://httpcrawler.sinaapp.com/hello.html";
*/

    int slen = strlen(url); 
    int len = slen + 1;

    sds_t *crawl_job_arg = (sds_t *)malloc(len + sizeof(sds_t)); /* will free in do_crawl */
    if(crawl_job_arg != NULL) {
        crawl_job_arg->len = len;
	bzero(crawl_job_arg->buffer, len);
        memcpy(crawl_job_arg->buffer, url, slen);

        job_t crawl_job;
        crawl_job.run = do_crawler;
        crawl_job.arg = (void *)crawl_job_arg;
        thread_pool_set_job(&thread_pool, &crawl_job);
	thread_counter_increase(&global_lock);
        thread_pool_post_job_semaphare(&thread_pool);
	/*
        job_t work_job;
        work_job.run = do_work;
        work_job.arg = NULL;
        thread_pool_set_job(&thread_pool, &work_job);
        thread_pool_post_job_semaphare(&thread_pool);
	*/
    }

    thread_t work_thread;
    thread_init(&work_thread);
    int ret = thread_create(&work_thread, NULL, do_work, NULL);
    if(ret != 0) {
	fprintf(stderr, "thread_create work faild, retval: %d.\n", ret);
	return ret;
    }

    thread_join(&work_thread);
    thread_pool_join(&thread_pool);

    thread_destroy(&work_thread);
    thread_pool_destroy(&thread_pool);

    printf("$$$$$$$$$$$$$$$$ start calc $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    calc_pagerank(&webgraph);
    gen_ind_cdf(&webgraph);
    gen_graphviz(&webgraph);
    gen_ind_pagerank(&webgraph, path);
    gen_top(&webgraph);

    thread_counter_destroy(&global_lock);
    ts_queue_destroy(&urlqueue);
    sem_destroy(&work_sem);
    destroy_webg(&webgraph);
    printf("===============================program over============================\n");

    return 0;
}


