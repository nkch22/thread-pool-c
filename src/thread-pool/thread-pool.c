#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include "../../include/thread-pool.h"
#include "../../include/task-queue.h"

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)


thread_pool_t *thread_pool_init(size_t size) {
	thread_pool_t* thread_pool = malloc(sizeof *thread_pool);
	thread_pool->stop = false;
	thread_pool->size = size;
	thread_pool->queue = task_queue_init();
	pthread_mutex_init(&thread_pool->queue_mutex, NULL);
	pthread_cond_init(&thread_pool->cond, NULL);

	int s;
	pthread_attr_t attr;

	s = pthread_attr_init(&attr);
	if (s != 0)
		handle_error_en(s, "pthread_attr_init");

	/* Allocate memory for pthread_create() arguments. */
	thread_pool->threads = calloc(size, sizeof(*thread_pool->threads));
	if (thread_pool->threads == NULL)
		handle_error("calloc");

	/* Create one thread for each command-line argument. */
	for (size_t tnum = 0; tnum < size; tnum++) {
		thread_pool->threads[tnum].thread_num = tnum + 1;

		/* The pthread_create() call stores the thread ID into
		corresponding element of tinfo[]. */
		s = pthread_create(&thread_pool->threads[tnum].thread_id, &attr,
						&worker_function, thread_pool);
		if (s != 0)
			handle_error_en(s, "pthread_create");
	}

	/* Destroy the thread attributes object, since it is no
	   longer needed. */
	s = pthread_attr_destroy(&attr);
	if (s != 0)
		handle_error_en(s, "pthread_attr_destroy");

	return thread_pool;
}

void thread_pool_dealloc(thread_pool_t* thread_pool) {
    thread_pool->stop = true;

    // wake up all waiting threads
    pthread_cond_broadcast(&thread_pool->cond);

    for (size_t i = 0; i < thread_pool->size; i++) {
        pthread_join(thread_pool->threads[i].thread_id, NULL);
    }

    task_queue_dealloc(thread_pool->queue);
    pthread_mutex_destroy(&thread_pool->queue_mutex);
    pthread_mutex_destroy(&thread_pool->stop_mutex);
    pthread_cond_destroy(&thread_pool->cond);
    free(thread_pool->threads);
    free(thread_pool);
}

void* worker_function(void* args)
{
    thread_pool_t *pool = (thread_pool_t*)args;
    while(!pool->stop || !task_queue_is_empty(pool->queue)) {
        pthread_mutex_lock(&pool->queue_mutex);
        while(!pool->stop && task_queue_is_empty(pool->queue)) {
            pthread_cond_wait(&pool->cond, &pool->queue_mutex);
        }
        if (pool->stop && task_queue_is_empty(pool->queue)) {
            pthread_mutex_unlock(&pool->queue_mutex);
            return NULL;
        }
        task_t task = task_queue_pop(pool->queue);
        pthread_mutex_unlock(&pool->queue_mutex);
        task();
    }

    return NULL;
}

int thread_pool_add_task(thread_pool_t *thread_pool, task_t task) {
    pthread_mutex_lock(&thread_pool->queue_mutex);
    int newSize = task_queue_push(thread_pool->queue, task);
    pthread_mutex_unlock(&thread_pool->queue_mutex);
    pthread_cond_signal(&thread_pool->cond);
    return newSize;
}
