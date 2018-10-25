#ifndef MALLOC_H
#define MALLOC_H

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>

#define MAX_PAGE_LIMIT 256
#define BIN_NUMBER 3
#define BIN_LIMIT 1024

#define FREELIST 0
#define USEDLIST 1

typedef int bool;

#define true 1
#define false 0

#define BIN64 0
#define BLOCK64 64
#define BIN128 1
#define BLOCK128 128
#define BIN512 2
#define BLOCK512 512

struct mem_meta {
  ssize_t size;
  char *startAddress;
};

/* Creates the BIN arena with ability to free the list and read the
used lists, along with a max page-data BIN_LIMIT */
struct arena {
  struct mem_meta freelist[BIN_NUMBER][BIN_LIMIT];
  struct mem_meta usedlist[BIN_NUMBER + 1][BIN_LIMIT];
  struct mem_meta page[MAX_PAGE_LIMIT];
};

static __thread struct arena thread_arena;
static __thread int alloc_requests[BIN_NUMBER + 1];
static __thread int free_requests[BIN_NUMBER];
static __thread int total_size;

// Main functions for the malloc library to pull data and functions from.

void *malloc(size_t size);

void free(void *ptr);

void *calloc(size_t nmemb, size_t size);

void *realloc(void *ptr, size_t size);

int get_size_of_bin(int bin_number);

int set_size(int size);

int set_list_index(int size);

void init_arena();

int get_first_free_from_list(int list_number, int size);

void* create_block_and_add_to_usedlist(int size,int iterator);

void malloc_stats_print();

int print_stats();

#endif // MALLOC_H
