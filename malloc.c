#include "malloc.h"

/*
 To each allocation additional data is added due to
 alignment requirements, this allows optional ability to
 add more than just one integer.
*/
static const size_t alignment = 8; /* bytes (>= 2*sizeof(size_t)) */

/*
A sentinel value prefixed to each allocation
*/
static const size_t sentinel = 0xBADC0DE;

/* output */
#define PPREFIX "malloc_count ### "

typedef struct MallocHeader {
  size_t size;
} MallocHeader;

void *calloc(size_t nmemb, size_t size) {
  return NULL;
}

void *malloc(size_t size) {
  // TODO: Validate size.
  if(thread_arena.page[0].size != -1 && thread_arena.page[0].startAddress == NULL ) {
    init_arena();
  }
  size_t allocSize = size + sizeof(MallocHeader);

  void *ret = mmap(0, allocSize, PROT_READ | PROT_WRITE,
                  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  assert(ret != MAP_FAILED);

  // We can't use printf here because printf internally calls `malloc` and thus
  // we'll get into an infinite recursion leading to a segfault.
  // Instead, we first write the message into a string and then use the `write`
  // system call to display it on the console.
  char buf[1024];
  snprintf(buf, 1024, "%s:%d malloc(%zu): Allocated %zu bytes at %p\n",
    __FILE__, __LINE__, size, allocSize, ret);
  write(STDOUT_FILENO, buf, strlen(buf) + 1);

  MallocHeader *hdr = (MallocHeader*) ret;
  hdr->size = allocSize;

  return ret + sizeof(MallocHeader);
}

void free(void *ptr) {
  MallocHeader *hdr = ptr - sizeof(MallocHeader);
  // We can't use printf here because printf internally calls `malloc` and thus
  // we'll get into an infinite recursion leading to a segfault.
  // Instead, we first write the message into a string and then use the `write`
  // system call to display it on the console.
  char buf[1024];
  snprintf(buf, 1024, "%s:%d free(%p): Freeing %zu bytes from %p\n",
    __FILE__, __LINE__, ptr, hdr->size, hdr);
  write(STDOUT_FILENO, buf, strlen(buf) + 1);
  munmap(hdr, hdr->size);
}

void *realloc(void *ptr, size_t size) {
  int *ptr_new;
  size_t oldsize;
  void* newptr;
  int i;

  // Allocate new memory (if needed) and copy the bits from old location to new.
  ptr_new = (int *)realloc(ptr, sizeof(int) * 3);
  *(ptr_new + 2) = 30;

  for(i = 0; i < 3; i++) {
    printf("%d ", *(ptr_new + i));
  }

  ptr = (char*)ptr - alignment;

  if (*(size_t*)((char*)ptr + alignment - sizeof(size_t)) != sentinel) {
      fprintf(stderr, PPREFIX "realloc(%p) has no sentinel !!! memory corruption?\n", ptr);
  }

  oldsize = *(size_t*)ptr;

  if (oldsize >= size) {
      /* keep old area, just reduce the size */
      *(size_t*)ptr = size;

      return (char*)ptr + alignment;
  } else {
      /* allocate new area and copy data */
      ptr = (char*)ptr + alignment;
      newptr = malloc(size);
      memcpy(newptr, ptr, oldsize);
      free(ptr);

      return newptr;
  }

  return NULL;;
}

// Main function for printing the malloc_stats
int main() {
  int* check;

	malloc_stats();

  printf("\n\n");

	malloc_stats_print();

  free(check);

	return 0;
}
