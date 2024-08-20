#include <stddef.h>
#include <stdbool.h>

typedef void* task_queue_t;
typedef void(*task_t)(void);

task_queue_t task_queue_init();
int task_queue_dealloc(task_queue_t queue);

int task_queue_push(task_queue_t queue, task_t task);
task_t task_queue_pop(task_queue_t queue);

bool task_queue_is_empty(task_queue_t queue);
