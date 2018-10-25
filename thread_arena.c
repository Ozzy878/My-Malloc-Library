#include "thread_arena.h"
#include "malloc.h"
#include "free.h"

static __thread block_info* list[NUM_BINS];

pthread_mutex_t mutex;
int num_arenas = 0;
int num_blocks[NUM_BINS] = {0};
int free_blocks[NUM_BINS] = {0};
int malloc_reqs[NUM_BINS] = {0};
int free_reqs[NUM_BINS] = {0};

// Malloc stats function for printing the data
void malloc_stats() {

    printf("Total size of arenas allocated with sbrk: %zu\n", num_arenas * PAGE_SZ);
    printf("Total number of bins: %d\n", NUM_BINS);

    for (int i = 0; i < NUM_BINS; ++i) {

        printf("------------BIN %d------------\n", i);
        printf("Total number of blocks: %d\n", num_blocks[i]);
        printf("Used blocks: %d\n", num_blocks[i] - free_blocks[i]);
        printf("Free blocks: %d\n", free_blocks[i]);
        printf("Total malloc requests: %d\n", malloc_reqs[i]);
        printf("Total free requests: %d\n", free_reqs[i]);
    }
}
