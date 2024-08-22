#include <stddef.h>
#include <pthread.h>

#include "task-queue.h"

typedef struct thread_info {
    pthread_t thread_id;
    int       thread_num;
} thread_info_t;

typedef struct {
    thread_info_t *threads;
    size_t size;
    task_queue_t queue;
    pthread_mutex_t queue_mutex;
    pthread_cond_t cond;
    bool stop;
    pthread_mutex_t stop_mutex;

} thread_pool_t;

thread_pool_t *thread_pool_init(size_t size);
void thread_pool_dealloc(thread_pool_t*);

void* worker_function(void* arg);

int thread_pool_add_task(thread_pool_t *thread_pool, task_t task);
