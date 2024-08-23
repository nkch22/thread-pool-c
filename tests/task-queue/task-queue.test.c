#include "../test.h"
#include "../../src/task-queue/task-queue.private.h"

void test_task_queue_init() {
    task_queue_t queue = task_queue_init();
    queue_t* casted = (queue_t*)queue;
    TEST_ASSERT(casted->size == 0);
    TEST_ASSERT(casted->head == NULL && casted->tail == NULL);
    task_queue_dealloc(queue);

    fprintf(stdout, "TEST %-60s \tPASSED\n", __func__);
}

void task_function() {
    printf("SOME TEXT\n");
}

void test_task_queue_push() {
    task_queue_t queue = task_queue_init();
    queue_t* casted = (queue_t*)queue;
    task_queue_push(queue, task_function);

    TEST_ASSERT(casted->size == 1);
    TEST_ASSERT(casted->head == casted->tail);
    TEST_ASSERT(casted->head->next == NULL && casted->head->next == NULL);

    task_queue_dealloc(queue);

    fprintf(stdout, "TEST %-60s \tPASSED\n", __func__);
}

void test_task_queue_pop() {
    task_queue_t queue = task_queue_init();
    queue_t* casted = (queue_t*)queue;
    task_queue_push(queue, task_function);
    task_queue_push(queue, task_function);

    task_queue_pop(queue);

    TEST_ASSERT(casted->size == 1);
    TEST_ASSERT(casted->head == casted->tail);
    TEST_ASSERT(casted->head->next == NULL && casted->tail->next == NULL);

    task_queue_dealloc(queue);

    fprintf(stdout, "TEST %-60s \tPASSED\n", __func__);
}

void run_all_tests() {
    printf("RUNNING %s TESTS\n", __FILE__);
    test_task_queue_init();
    test_task_queue_push();
    test_task_queue_pop();
}

int main() {
   run_all_tests();
   return 0;
}
