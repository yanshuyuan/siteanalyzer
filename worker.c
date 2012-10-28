#include "worker.h"

extern thread_pool_t thread_pool;
extern sem_t work_sem;
extern ts_queue_t urlqueue;
extern webg_t webgraph;
extern thread_counter_t thread_idle_count;
extern thread_counter_t global_lock;

int check(char *buf)
{
    int len = strlen(buf);
    const char *domain = "http://125.211.218.8/";
    const int domain_len = strlen(domain);
    if(len >= domain_len && !memcmp(buf, domain, domain_len) == 0) return 0;
    if((len > 5 && memcmp(buf + len - 5, ".html", 5) == 0)
	|| (len > 4 && memcmp(buf + len - 4, ".htm", 4) == 0)) return 1;
    return 0;
}

int filter(list_t *olist, list_t *nlist)
{
    /* false positive rate: 1 / 10^6 */
    BF bf;
    if(bloom_init(&bf, list_size(olist) * 60, 6, sdbm_hash, sax_hash, elf_hash, js_hash, bkdr_hash, djb_hash)) {
        elem_t *iter;
        for(iter = list_first(olist); iter != list_end(olist); iter = list_next(iter)) {
            /* only crawl local link and delete reduplicated url */
            if(check(iter->buffer) && !bloom_check(&bf, iter->buffer)) { 
                list_push_back(nlist, iter->buffer, iter->len);
                bloom_add(&bf, iter->buffer);
            }
        }
        bloom_destroy(&bf);
	return 1;
    } 
    return 0;
}

void *do_exit(void *arg) 
{
    job_arg_t *thread_arg = (job_arg_t *)arg;
    printf("threadId: %d\n", thread_arg->thread_item->id);
    thread_set_idle(thread_arg->thread_item, THREAD_IDLE);
    return 0;
}



void printThreadInfo()
{
    printf("=========thread info=========================\n");
    int i;
    for(i = 0; i < thread_pool.thread_count; i++) {
	printf("thread[%d].job_done: %d\n", i, thread_counter_get(&thread_pool.thread_items[i].job_done));
    }
    printf("=========thread info end==========================\n");
}

void *do_work(void *arg) /* only one thread run this func */
{
    printf("=====================do_work start================================\n");
    int report = 0;
    link_info_t link_info; 
    while(thread_counter_get(&global_lock) != 0 && webgraph.vertex_size < VERTEX_DEFAULT) {
	/* every loop is a vertex's edges inserted and add a web site crawl url into job_queue */
	sem_wait(&work_sem);
    	printf("=========get work=================\n");
	if(ts_queue_size(&urlqueue) > 0) {
	    ts_queue_top(&urlqueue, &link_info);
	    
	    char *url = link_info.url;
	    list_t *link_url = link_info.link_url;
	    
	    insert_edge(&webgraph, url, link_url);
	    
	    elem_t *iter;
	    /* send crawl job */
	    vertex_info_t *vertex_info;
	    printf("=====================create job=========================\n");
	    for(iter = list_first(link_url); iter != list_end(link_url); iter = list_next(iter)) {
	        vertex_info = find_vertex_info(&webgraph, iter->buffer);
	        /*
	    
	        printf("SSSSSS-===url: %s====\n", iter->buffer);
	        printf("SSSSSS-===vid: %d====\n", vertex->index.vid);
	        printf("SSSSSS-===url vertex in: %d=====\n", vertex->in);
	    	*/
	        /* if vertex_info->in is true, the vertex has crawl */
	        if(!vertex_info->in) {
	            job_t job;
                    job.run = do_crawler;
                    sds_t *job_arg; /* will free in do_crawler  module */

		    /* job_arg need free in do_crawler */
                    if((job_arg = (sds_t *)malloc(sizeof(sds_t) + iter->len)) != NULL) {
                        job_arg->len = iter->len;
                        memcpy(job_arg->buffer, iter->buffer, iter->len);
                        job.arg = (void *)job_arg;
    	                thread_pool_set_job(&thread_pool, &job);
	    	        vertex_info->in = 1;
    			thread_counter_increase(&global_lock);
                        thread_pool_post_job_semaphare(&thread_pool);
	            } else {
	    	    	fprintf(stderr, "Error, allocate memory failed in func 'do_work'.\n");
	       	    }
	        }
	    }
	    printf("=====================create job over=========================\n");
	    free(link_info.url); /* malloc in do_work */
	    list_destroy(link_info.link_url); /* malloc in do_work */
	    free(link_info.link_url); /* malloc in do_work */
    	    printf("=========get work end=================\n");
	    thread_counter_decrease(&global_lock);
	    
	    printf("=====================do work over=========================\n");
	} else {
    	    printf("=========check over=================\n");
	}
        printf("==========job queue size: %d===============================\n", ts_queue_size(&thread_pool.job_queue));
        printf("==========urlqueue size: %d===============================\n", ts_queue_size(&urlqueue));
        printf("==========job done: %d===============================\n", thread_counter_get(&thread_pool.job_done));
        printf("==========vertex size: %d===============================\n", webgraph.vertex_size);
        printf("==========idle thread: %d===============================\n", thread_counter_get(&thread_pool.thread_idle));
    	printf("=========global_lock count: %d====================================\n", thread_counter_get(&global_lock));
        if(report++ % 100 == 99) {
            printThreadInfo();
        }
    }
    printf("-------------------do work post exit-----------------------------\n");
    /* tell thread_pool thread stop */
    int i;
    for(i = 0; i < thread_pool.thread_count; i++) {
	job_t exit_job;
	exit_job.run = do_exit;
	exit_job.arg = NULL;
	thread_pool_set_job(&thread_pool, &exit_job);
        thread_pool_post_job_semaphare(&thread_pool);
    }

    printWebg(&webgraph);
    
    printf("-------------------do work run over-----------------------------\n");
    return 0;
}

void *do_crawler(void *arg) 
{
    job_arg_t *thread_arg = (job_arg_t *)arg;
    sds_t *job_arg = (sds_t *)thread_arg->arg;

    vertex_t *vertex;
    vertex = find_vertex(&webgraph, job_arg->buffer);
    char url[4096];
    bzero(url, 4096);
    memcpy(url, job_arg->buffer, job_arg->len);
    int timeout = 3;
    int retval;

    link_crawler_t crawler;
    list_t link_list;
    do {
        list_init(&link_list);
            /* check the path is already crawled */
                /* crawl path */
        printf("---------threadID: %d(%lu) crawler url: %s---------------\n", thread_arg->thread_item->id, thread_arg->thread_item->threadinfo, url);
	retval = crawler_crawl(&crawler, url, &link_list);
        if(retval == CRAWLER_OK) {
            /* enque crawled list */
        	link_info_t link_info; 
            int len = job_arg->len + 1; /* append '\0' */
            if((link_info.url = (char *)malloc(len)) != NULL) {
                bzero(link_info.url, len);
                /* copy url into relation */
                memcpy(link_info.url, job_arg->buffer, job_arg->len); /* link_info.url need free in do_work */
                if((link_info.link_url = (list_t *)malloc(sizeof(list_t))) != NULL) {
                    list_init(link_info.link_url); /*  link_info.link_url need destroy in do_work */
                    if(filter(&link_list, link_info.link_url)) { /* filter reduplicate link and no need for this test link */
            	        ts_queue_push(&urlqueue, &link_info, sizeof(link_info_t));
    			thread_counter_increase(&global_lock); /* push urlqueue need post work to deal it */
    			sem_post(&work_sem);
        		    printf("---------threadID: %d(%lu) crawler url: %s success---------------\n",
            	    		 thread_arg->thread_item->id, thread_arg->thread_item->threadinfo, url);
                    } else {
        		    printf("---------threadID: %d(%lu) crawler url: %s link_list is zero---------------\n", 
            			thread_arg->thread_item->id, thread_arg->thread_item->threadinfo, url);
            	}
                } else {
        		printf("---------threadID: %d(%lu) crawler url: %s failed---------------\n", 
            			thread_arg->thread_item->id, thread_arg->thread_item->threadinfo, url);
                	fprintf(stderr, "Error, allocate memory failed in func 'do_crawler'.\n");
                }
            } else {
        	    printf("---------threadID: %d(%lu) crawler url: %s failed---------------\n", 
            			thread_arg->thread_item->id, thread_arg->thread_item->threadinfo, url);
                fprintf(stderr, "Error, allocate memory failed in func 'do_crawler'.\n");
            }
        } else if(retval == CRAWLER_TIMEOUT) {
	    timeout--;
            printf("---------threadID: %d(%lu) crawler url: %s timeout, retry times: %d---------------\n", 
            			thread_arg->thread_item->id, thread_arg->thread_item->threadinfo, url, timeout);
            fprintf(stderr, "Error: crawler url '%s' failed.\n", url);
	    
	} else {
        	printf("---------threadID: %d(%lu) crawler url: %s failed---------------\n", 
            			thread_arg->thread_item->id, thread_arg->thread_item->threadinfo, url);
            fprintf(stderr, "Error: crawler url '%s' failed.\n", url);
        }
        list_destroy(&link_list);
    } while((retval == CRAWLER_TIMEOUT || retval == CRAWLER_FAILED || retval == CRAWLER_UNREACH) && timeout > 0);

    printf("================================send post job============================\n");
    if(job_arg != NULL) { free(job_arg); }
    thread_counter_decrease(&global_lock); /* finish urlqueue need post work to check idle */
    sem_post(&work_sem);
    printf("================================send check job============================\n");
    return 0;
}
