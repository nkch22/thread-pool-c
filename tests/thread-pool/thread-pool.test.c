#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "../../include/thread-pool.h"
#include "../test.h"

static const char temp_file[] = "tmp.txt";
static const char target_string[] = "Task finished\n";
static FILE *tmp = NULL;

pthread_mutex_t mutex;

void dummy_task(void) {
    pthread_mutex_lock(&mutex);
    fprintf(tmp, "%s", target_string);
    pthread_mutex_unlock(&mutex);
}

bool check_test_output(size_t number_of_tasks, FILE *output_file) {
    size_t count = 0;
    size_t length = strlen(target_string) + 1;
    char buffer[length];
    fseek(output_file, 0, SEEK_SET);
    while (fgets(buffer, sizeof(buffer), output_file) != NULL) {
        char *pos = buffer;
        while ((pos = strstr(pos, target_string)) != NULL) {
            count++;
            pos += length;
        }
    }
    return count == number_of_tasks;
}

void test_thread_pool_init(void) {
    const size_t number_of_threads = 5;
    thread_pool_t *pool = thread_pool_init(number_of_threads);

    TEST_ASSERT(pool->size == number_of_threads);
    TEST_ASSERT(pool->stop == false);
    TEST_ASSERT(pool->queue != NULL);

    fprintf(stdout, "TEST %-60s \tPASSED\n", __func__);

    thread_pool_dealloc(pool);
}

void test_thread_pool_add_task_simple(void) {
    const size_t number_of_threads = 5;
    thread_pool_t *pool = thread_pool_init(number_of_threads);

    TEST_ASSERT(task_queue_is_empty(pool->queue));

    int size = thread_pool_add_task(pool, dummy_task);

    TEST_ASSERT(size == 1);

    sleep(1);

    // assume the tasks are completed
    TEST_ASSERT(task_queue_is_empty(pool->queue));

    thread_pool_dealloc(pool);

    fprintf(stdout, "TEST %-60s \tPASSED\n", __func__);
}

void test_thread_pool_add_task_multiple_tasks(void) {
    const size_t number_of_threads = 5;
    const size_t number_of_tasks = 10;
    thread_pool_t *pool = thread_pool_init(number_of_threads);

    tmp = fopen(temp_file, "w+");

    for (size_t i = 0; i < number_of_tasks; i++) {
        thread_pool_add_task(pool, dummy_task);
    }
    thread_pool_dealloc(pool);

    TEST_ASSERT(check_test_output(number_of_tasks, tmp));

    fclose(tmp);

    fprintf(stdout, "TEST %-60s \tPASSED\n", __func__);
}

void test_thread_pool_dealloc_executes_all_tasks_before_dealloc(void) {
    const size_t number_of_threads = 1;
    const int number_of_tasks = 5;
    thread_pool_t *pool = thread_pool_init(number_of_threads);

    tmp = fopen(temp_file, "w+");

    for (int i = 0; i < number_of_tasks; i++) {
        thread_pool_add_task(pool, dummy_task);
    }
    thread_pool_dealloc(pool);

    TEST_ASSERT(check_test_output(number_of_tasks, tmp));

    fclose(tmp);

    fprintf(stdout, "TEST %-60s \tPASSED\n", __func__);
}

void run_all_tests(void) {
    tmp = fopen("/dev/null", "w");
    printf("RUNNING %s TESTS\n", __FILE__);
    test_thread_pool_init();
    test_thread_pool_add_task_simple();
    fclose(tmp);
    test_thread_pool_add_task_multiple_tasks();
    test_thread_pool_dealloc_executes_all_tasks_before_dealloc();
    remove(temp_file);
}

int main() {
    run_all_tests();
    return 0;
}
