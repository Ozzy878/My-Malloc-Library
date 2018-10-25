#include "malloc.h"

void * realloc(void *ptr, size_t size) {

    if (!ptr) {

      return malloc(size);
    }
    int iterator, iterator2 = 0;

    for (iterator = 0; iterator < BIN_NUMBER; iterator++) {
        for (iterator2 = 0; iterator2 < BIN_LIMIT; iterator2++) {
            if (thread_arena.usedlist[iterator][iterator2].startAddress == ptr) {
                if (thread_arena.usedlist[iterator][iterator2].size >= size) {
                    alloc_requests[set_list_index(set_size((int) size))]++;

                    return ptr;
                }
                break;
            }
        }
        if (iterator2 < BIN_NUMBER) {

          break;
        }
    }

    if (iterator == BIN_NUMBER) {

      return malloc(size);
    }

    void *ret = malloc(size);
    memcpy(ret, ptr, (size_t) thread_arena.usedlist[iterator][iterator2].size);

    free(ptr);
    free_requests[set_list_index(set_size((int) size))]--;

    return ret;
}
