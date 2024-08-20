#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(condition) \
  if (!(condition)) { \
    fprintf(stderr, "Assertion failed: %s at %s:%d\n", #condition, __FILE__, __LINE__); \
    exit(1); \
  }
