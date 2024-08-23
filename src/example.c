#include <stdio.h>
#include <unistd.h>
#include "../include/thread-pool.h"

void task1() {
    puts("wopwop");
    sleep(1);
}

int main(void) {
    thread_pool_t *pool = thread_pool_init(3);

    for (size_t i = 0; i < 300; i++) {
        thread_pool_add_task(pool, task1);
    }

    thread_pool_dealloc(pool);

    return 0;
}
