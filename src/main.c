#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/thread-pool.h"

void task1() {
    puts("wowop");
}

void task2() {
    sleep(2);
    puts("Second task completed!");
}

void task3() {
    sleep(2);
    puts("Third task completed!");
}

void task4() {
    sleep(2);
    puts("Fourth task completed!");
}

void task5() {
    sleep(2);
    puts("Fifth task completed!");
}

int main(void) {
    thread_pool_t *pool = thread_pool_init(3);

    for (size_t i = 0; i < 300; i++) {
        thread_pool_add_task(pool, task1);
    }

    thread_pool_dealloc(pool);

    return 0;
}
