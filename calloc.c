#include "malloc.h"

void *calloc(size_t nmemb, size_t size) {
    size_t required_size = nmemb * size;
    void *ptr = malloc((size_t) required_size);
    memset(ptr, 0, (size_t) required_size);

    return ptr;
}
