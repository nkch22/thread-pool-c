#!/bin/bash

# used to check the thread pool on deadlocks
for i in {1..5}; do
	./build/thread-pool-example
done
