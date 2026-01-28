#define _DEFAULT_SOURCE

#include "bmalloc.h"

#include <errno.h>
#include <unistd.h>

static block_header_t* free_list_head = NULL;

static heap_t* heap = NULL;
void init_heap() {
        size_t initial_size = HEAP_CHUNK_SIZE;

        void* heap_start = sbrk(initial_size);
        if (heap_start == (void*)-1) {
                return;
        }

        heap = (heap_t*)heap_start;

        heap->total_size = initial_size;
        heap->free_space_start = (char*)heap_start + sizeof(heap_t);
        heap->free_space_remaining = initial_size;
        heap->blocks = free_list_head;
}

void* bmalloc(size_t size) {
        if (heap == NULL) {
                init_heap();
        }

        size_t aligned_size = ALIGN(size);

        block_header_t* current = heap->blocks;
        block_header_t* previous = heap->blocks;
        while (current != NULL) {
                if (current->is_free == 1 && current->size >= aligned_size) {
                        current->is_free = 0;
                        current->magic = MAGIC_ALLOCATED;
                        return (void*)((char*)current + sizeof(block_header_t));
                }

                previous = current;
                current = current->next;
        }

        while (heap->free_space_remaining <
               aligned_size + sizeof(block_header_t)) {
                size_t new_chunk_size = HEAP_CHUNK_SIZE;
                void* new_heap_data = sbrk(new_chunk_size);
                if (new_heap_data == (void*)-1) {
                        return NULL;
                }
                heap->total_size += new_chunk_size;
                heap->free_space_remaining += new_chunk_size;
        }

        block_header_t* new_block = (block_header_t*)heap->free_space_start;
        new_block->is_free = 0;
        new_block->size = aligned_size;
        new_block->magic = MAGIC_ALLOCATED;
        new_block->next = NULL;

        heap->free_space_start = (char*)heap->free_space_start +
                                 sizeof(block_header_t) + aligned_size;

        if (previous == NULL) {
                heap->blocks = new_block;
        } else {
                previous->next = new_block;
        }

        heap->free_space_remaining -= (sizeof(block_header_t) + aligned_size);
        return (void*)((char*)new_block + sizeof(block_header_t));
}

void bfree(void* ptr) {
        block_header_t* metadata =
            (block_header_t*)((char*)ptr - sizeof(block_header_t));
        if (metadata->magic == MAGIC_FREED || metadata->magic == MAGIC_FREE) {
                // probably do more extensive error handling here
                errno = EINVAL;
                return;
        }

        metadata->magic = MAGIC_FREED;
        metadata->is_free = 1;

        return;
}
