CC = gcc
INCLUDE_DIRS = -I./include
LIBS = -lpthread
CFLAGS = -g -Wall -Wextra -pedantic $(LIBS) $(INCLUDE_DIRS)
BINARY_NAME = thread_pool

TESTS_BINARIES = queue-tests thread-pool-tests

all: main.o thread-pool.o task-queue.o
	$(CC) $(CFLAGS) $^ -o $(BINARY_NAME)

thread-pool-tests: thread-pool.o task-queue.o thread-pool.test.o
	$(CC) $(CFLAGS) $^ -o $@

thread-pool.test.o: ./tests/thread-pool/thread-pool.test.c
	$(CC) $(CFLAGS) $^ -c

thread-pool.o: ./src/thread-pool/thread-pool.c
	$(CC) $(CFLAGS) $^ -c

queue-tests: task-queue.o task-queue.test.o
	$(CC) $(CFLAGS) $^ -o $@

task-queue.test.o: ./tests/task-queue/task-queue.test.c
	$(CC) $(CFLAGS) $^ -c

task-queue.o: ./src/task-queue/task-queue.c
	$(CC) $(CFLAGS) $^ -c

main.o: ./src/main.c
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o $(BINARY_NAME) $(TESTS_BINARIES)
