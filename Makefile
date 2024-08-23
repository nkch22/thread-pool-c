CC = gcc
INCLUDE_DIRS = -I./include
LIBS = -lpthread
CFLAGS = -g -Wall -Wextra -pedantic $(LIBS) $(INCLUDE_DIRS)
BINARY_NAME = thread_pool_example

BUILD_DIR = build
$(shell mkdir -p $(BUILD_DIR))

LIB_NAME = libthread-pool

all: $(BUILD_DIR)/example.o $(BUILD_DIR)/$(LIB_NAME).a
	$(CC) $(CFLAGS) $(BUILD_DIR)/example.o -o $(BUILD_DIR)/$(BINARY_NAME) -L$(BUILD_DIR) -l:$(LIB_NAME).a
	rm -f $(BUILD_DIR)/*.o

tests: $(BUILD_DIR)/test-runner.o $(BUILD_DIR)/thread-pool.o $(BUILD_DIR)/task-queue.o $(BUILD_DIR)/thread-pool.test.o $(BUILD_DIR)/task-queue.test.o
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/$@
	rm -f $(BUILD_DIR)/*.o

$(BUILD_DIR)/$(LIB_NAME).a: $(BUILD_DIR)/thread-pool.o $(BUILD_DIR)/task-queue.o
	ar rcs $@ $^

$(BUILD_DIR)/test-runner.o: ./tests/test-runner.c
	$(CC) $(CFLAGS) $^ -c -o $@

$(BUILD_DIR)/thread-pool.test.o: ./tests/thread-pool/thread-pool.test.c
	$(CC) $(CFLAGS) $^ -c -o $@

$(BUILD_DIR)/thread-pool.o: ./src/thread-pool/thread-pool.c
	$(CC) $(CFLAGS) $^ -c -o $@

$(BUILD_DIR)/task-queue.test.o: ./tests/task-queue/task-queue.test.c
	$(CC) $(CFLAGS) $^ -c -o $@

$(BUILD_DIR)/task-queue.o: ./src/task-queue/task-queue.c
	$(CC) $(CFLAGS) $^ -c -o $@

$(BUILD_DIR)/example.o: ./src/example.c
	$(CC) $(CFLAGS) $^ -c -o $@

clean:
	rm -rf $(BUILD_DIR)
