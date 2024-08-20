#include <stdlib.h>
#include "task-queue.private.h"

task_queue_t task_queue_init() {
    queue_t* queue = malloc(sizeof *queue);
    if (queue == NULL) return NULL;

    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;

    return queue;
}

int task_queue_dealloc(task_queue_t queue) {
    Node* current = ((queue_t*)queue)->head;
    while(current) {
        Node* tmp = current->next;
        free(current);
        current = tmp;
    }
    free(queue);
    return 1;
}

int task_queue_push(task_queue_t queue, task_t task) {
    Node* new_node = malloc(sizeof *new_node);
    new_node->task = task;
    if(new_node == NULL) return -1;

    queue_t* casted = (queue_t*)queue;
    if (casted->head == NULL) {
        casted->head = new_node;
        casted->tail = new_node;
        casted->size += 1;
        return casted->size;
    }

    Node* tmp = casted->tail;
    casted->tail = new_node;
    tmp->next = new_node;
    casted->size += 1;
    return casted->size;
}

task_t task_queue_pop(task_queue_t queue) {
    queue_t* casted = (queue_t*)queue;
    if (casted->head == NULL) return NULL;

    // single element in list
    if (casted->head == casted->tail) {
        task_t task = casted->head->task;
        free(casted->head);
        casted->head = NULL;
        casted->tail = NULL;
        casted->size -= 1;
        return task;
    }

    task_t task = casted->head->task;
    Node* tmp = casted->head;
    casted->head = casted->head->next;
    free(tmp);
    casted->size -= 1;
    return task;
}

bool task_queue_is_empty(task_queue_t queue) {
    return ((queue_t*)queue)->size == 0;
}
