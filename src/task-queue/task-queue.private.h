#include "../../include/task-queue.h"

typedef struct Node_t {
    task_t task;
    struct Node_t* next;
} Node;

typedef struct queue {
    Node* head;
    Node* tail;
    size_t size;
} queue_t;
