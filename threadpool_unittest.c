#include "thread_pool.h"


typedef struct _job_arg {
    thread_pool_t *thread_pool;
    void *arg;
} job_arg_t;

void *do_square(void *arg)
{
    printf("do_square ptr: %p\n", arg);
    job_arg_t *job_arg = (job_arg_t *) arg;
    thread_pool_t *thread_pool = job_arg->thread_pool;
    printf("do_square pool ptr: %p\n", thread_pool);
    int n = *(int *)job_arg->arg;
    printf("square %d is: %d\n", n, n * n);
}

void *do_double(void *arg)
{
    printf("do_double ptr: %p\n", arg);
    job_arg_t *job_arg = (job_arg_t *) arg;
    thread_pool_t *thread_pool = job_arg->thread_pool;
    printf("do_double pool ptr: %p\n", thread_pool);
    int n = *(int *)job_arg->arg;
    printf("double %d is: %d\n", n, n + n);
}

int main()
{
    thread_pool_t thread_pool;
    printf("pointer: %p\n", thread_pool);
    thread_pool_init(&thread_pool, 10);
    thread_pool_run(&thread_pool);

    printf("---------------------------------\n");
    int n = 3;
    job_arg_t job_arg;
    job_arg.thread_pool = &thread_pool;
    job_arg.arg = (void *)&n;

    job_t job1;
    job1.run = do_square;
    printf("%p\n", job1.run);
    job1.arg = (void *)&job_arg;
    printf("%p\n", job1.arg);
    thread_pool_set_job(&thread_pool, &job1);

    job_t job2;
    job2.run = do_double;
    printf("%p\n", job2.run);
    job2.arg = (void *)&job_arg;
    printf("%p\n", job2.arg);
    thread_pool_set_job(&thread_pool, &job2);

    thread_pool_post_job_semaphare(&thread_pool);
    sleep(5);
    thread_pool_post_job_semaphare(&thread_pool);

    sleep(5);

    thread_pool_join(&thread_pool);


    thread_pool_destroy(&thread_pool);
	


}
