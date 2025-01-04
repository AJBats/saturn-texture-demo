#include "allocator/memalloc.h"
#include <malloc.h>

void* mem_alloc(size_t n) {
  return malloc(n);
}

void mem_free(void* mem) {
  free(mem);
}
