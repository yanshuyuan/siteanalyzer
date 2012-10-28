#ifndef _JOB
#define _JOB

#include "element.h"
#include <pthread.h>
#include "thread.h"

typedef void *(*start_routine_t) (void *);

typedef struct _job {
    start_routine_t run; 
    void *arg;
} job_t;

typedef struct _job_arg {
    thread_t *thread_item;
    void *arg;
} job_arg_t;

#endif

