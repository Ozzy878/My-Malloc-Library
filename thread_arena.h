
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>

#ifndef THREAD_ARENA_H
#define THREAD_ARENA_H

#define PAGE_SZ sysconf(_SC_PAGESIZE)
#define INFO_SZ sizeof(struct _block_info)
#define ARENA_SZ sizeof(struct _arena_info)
#define CALLER_ADDR __builtin_return_address(0)
#define NUM_BINS 4

typedef enum {
    BIN16 = 0,
    BIN64 = 1,
    BIN512 = 2,
    BINXL = 3

} bin_type;

typedef struct _block_info {
    size_t size;
    struct _block_info *next;
} block_info;

typedef struct _arena_info {
    size_t size;
    size_t offset;
} arena_t;

block_info* add_to_list(block_info* block, bin_type bin);

block_info* get_head(bin_type bin);

bin_type size_to_bin(size_t size);

size_t bin_to_size(bin_type bin);

arena_t* alloc_arena();

void *memset(void* s, int c, size_t n);

void *memcpy(void *dest, const void *src, size_t n);


#endif // THREAD_ARENA_H
