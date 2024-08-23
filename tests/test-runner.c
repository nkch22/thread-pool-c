void run_all_thread_pool_tests(void);
void run_all_task_queue_tests(void);

void run_all_tests() {
    run_all_task_queue_tests();
    run_all_thread_pool_tests();
}

int main() {
    run_all_tests();
    return 0;
}
