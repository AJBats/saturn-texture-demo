#include "allocator/memalloc.h"
//#include <mm/tlsf.h>

void* mem_alloc(size_t n) {
    (void)n;
    //tlsf_t const handle = master_state()->tlsf_handles[TLSF_HANDLE_USER];
    
    //return tlsf_malloc(handle, n);
    return 0;
}

void mem_free(void* mem) {
    (void)mem;
    //tlsf_t const handle = master_state()->tlsf_handles[TLSF_HANDLE_USER];

    //tlsf_free(handle, addr);
}
