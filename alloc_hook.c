#include <stdio.h>
#include "malloc.h"

/* Prototypes for our hooks.  */
static void my_init_hook(void);
static void *my_malloc_hook(size_t size, const void *caller);
static void my_free_hook(void* ptr, const void *caller);
/* Variables to save original hooks. */
static void *(*old_malloc_hook)(size_t, const void *);
static void (*old_free_hook)(void*, const void *);
/* Override initializing hook from the C library. */

static void my_init_hook(void) {
    old_malloc_hook = __malloc_hook;
    __malloc_hook = my_malloc_hook;
    old_free_hook = __free_hook;
    __free_hook = my_free_hook;
}

static void *my_malloc_hook(size_t size, const void *caller) {
    void *result;
    /* Restore all old hooks */
    __malloc_hook = old_malloc_hook;
    /* Call recursively */
    result = malloc(size);
    /* Save underlying hooks */
    old_malloc_hook = __malloc_hook;
    /* printf() might call malloc(), so protect it too. */
    printf("malloc(%u) called from %p returns %p\n",
    (unsigned int) size, caller, result);
    /* Restore our own hooks */
    __malloc_hook = my_malloc_hook;

    return result;
}

static void my_free_hook(void* ptr, const void *caller) {
    __free_hook = old_free_hook;

    free(ptr);
    printf("free(%p) called from %p returns\n", ptr, caller);

    old_free_hook = __free_hook;
    __free_hook = my_free_hook;
}

int main(void) n{
    my_init_hook();
    char *p;
    
    for (int i = 1; i <= 10; ++i) {
        p = malloc(1<<i);
        free(p);
    }

    return 0;
}
