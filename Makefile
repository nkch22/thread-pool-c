CC = gcc
INCLUDE_DIRS = -I./include
CFLAGS = -g -Wall -Wextra -Werror -pedantic $(INCLUDE_DIRS)


all: main.o
	$(CC) $(CFLAGS) $^ -o async-engine

main.o: ./src/main.c
	$(CC) $(CFLAGS) $^ -c
