#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <stddef.h>

void* mem_alloc(size_t n);
void mem_free(void* mem);


#endif // MEM_ALLOC_H
