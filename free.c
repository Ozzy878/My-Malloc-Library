#include "malloc.h"

void free(void *ptr) {
    int iterator, iterator2, iterator3;

    for (iterator = 0; iterator < BIN_NUMBER; iterator++) {
        for (iterator2 = 0; iterator2 < BIN_LIMIT; iterator2++) {

            if (thread_arena.usedlist[iterator][iterator2].startAddress == ptr) {
                free_requests[iterator]++;
                // search unused block in freelist
                for (iterator3 = 0; iterator3 < BIN_LIMIT; iterator3++) {
                    if (thread_arena.freelist[iterator][iterator3].startAddress == NULL) {
                        break;
                }
            }

                thread_arena.freelist[iterator][iterator3].startAddress = thread_arena.usedlist[iterator][iterator2].startAddress;
                thread_arena.usedlist[iterator][iterator3].startAddress = NULL;
            }
        }
    }
    for (iterator = 0; iterator < BIN_LIMIT; iterator++) {
        if (thread_arena.usedlist[BIN_NUMBER][iterator].startAddress == ptr) {
            free_requests[BIN_NUMBER]++;
            munmap(ptr, (size_t) thread_arena.usedlist[BIN_NUMBER][iterator].size);
            total_size -= thread_arena.usedlist[BIN_NUMBER][iterator].size;
            thread_arena.usedlist[BIN_NUMBER][iterator].startAddress = NULL;
            thread_arena.usedlist[BIN_NUMBER][iterator].size = -1;
        }
    }
}
