#ifndef BMALLOC_H
#define BMALLOC_H

#include <stddef.h>

typedef struct block_header {
        size_t size;
        int is_free;
        unsigned int magic;
        struct block_header* next;
} block_header_t;

typedef struct heap {
    void* free_space_start;
    size_t total_size;
    size_t free_space_remaining;
    struct block_header* blocks;
} heap_t;

#define MAGIC_ALLOCATED 0xDEADBEEF
#define MAGIC_FREE 0xFEEDFACE
#define MAGIC_FREED 0xDEADDEAD
#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

#define HEAP_CHUNK_SIZE (1024 * 1024)
#define DEFAULT_BLOCK_SIZE (1024 * 16)

void init_heap();
void* bmalloc(size_t size);
void bfree(void* ptr);
void* brealloc(void* ptr, size_t size);

#endif
