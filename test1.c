//
// Created by waldrons on 10/11/18.
//

#include "malloc.h"

int main(int argc, char **argv) {
    size_t size = 12;
    void *mem = malloc(size);

    printf("Successfully malloc'd %zu bytes at addr %p\n", size, mem);
    assert(mem != NULL);
    free(mem);

    printf("Successfully free'd %zu bytes from addr %p\n", size, mem);

    return 0;
}
