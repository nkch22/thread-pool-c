#!/bin/bash

# used to check the thread pool on deadlocks
for i in {1..1000}; do
	./thread_pool
done
