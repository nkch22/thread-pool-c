CC = gcc
INCLUDE_DIRS = -I./include
CFLAGS = -g -Wall -Wextra -Werror -pedantic $(INCLUDE_DIRS)


all: main.o
	$(CC) $(CFLAGS) $^ -o async-engine

queue-test: task-queue.o task-queue.test.o
	$(CC) $(CFLAGS) $^ -o queue-tests

task-queue.test.o: ./tests/task-queue/task-queue.test.c
	$(CC) $(CFLAGS) $^ -c

task-queue.o: ./src/task-queue/task-queue.c
	$(CC) $(CFLAGS) $^ -c

main.o: ./src/main.c
	$(CC) $(CFLAGS) $^ -c
