#include "malloc.h"


// This helper gets the size of the bins with the corressponding blocks
int get_size_of_bin(int bin_number) {
  if (bin_number == BIN64) {
    return BLOCK64;
  } else if (bin_number == BIN128) {
    return BLOCK128;
  } else if (bin_number == BIN512) {
    return BLOCK512;
  }
  return 0;
}

// Sets the size of the blocks
int set_size(int size) {
  if (size <= BLOCK64) {
    return BLOCK64;
  }
  if (size <= BLOCK128) {
    return BLOCK128;
  }
  if (size <= BLOCK512) {
    return BLOCK512;
  }

  return BLOCK512 + 1;
}

// Lists to bin indexes according to the size
int set_list_index(int size) {
  if (size == BLOCK64) {
    return BIN64;
  }
  if (size == BLOCK128) {
    return BIN128;
  }
  if (size == BLOCK512) {
    return BIN512;
  }
  return BIN512 + 1;
}

// Initiates the arena
void init_arena() {
  int iterator, iterator2;

  for (iterator = 0; iterator < BIN_NUMBER + 1; iterator++) {
    alloc_requests[iterator] = 0;
    free_requests[iterator] = 0;
  }
  total_size = 0;

  for (iterator = 0; iterator < MAX_PAGE_LIMIT; iterator++) {
    thread_arena.page[iterator].size = -1;
    thread_arena.page[iterator].startAddress = NULL;
  }
  for (iterator = 0; iterator < BIN_NUMBER; iterator++) {
    for (iterator2 = 0; iterator2 < BIN_LIMIT; iterator2++) {
      thread_arena.freelist[iterator][iterator2].size = get_size_of_bin(iterator);
      thread_arena.usedlist[iterator][iterator2].size = get_size_of_bin(iterator);
      thread_arena.freelist[iterator][iterator2].startAddress = NULL;
      thread_arena.usedlist[iterator][iterator2].startAddress = NULL;
    }
  }
  for (iterator = 0; iterator < BIN_LIMIT; iterator++) {
    thread_arena.usedlist[BIN_NUMBER][iterator].size = -1;
    thread_arena.usedlist[BIN_NUMBER][iterator].startAddress = NULL;
  }
}

// Retrieves the the of the bin numbers for the first freelist requested
int get_first_free_from_list(int list_number, int size) {
  int bin_number,iterator;

  if(size == BLOCK64)	{
    bin_number = BIN64;
  }
  if(size == BLOCK128) {
    bin_number = BIN128;
  }
  if(size == BLOCK512) {
    bin_number = BIN512;
  }
  if(size > BLOCK512) {
    bin_number = BIN512 + 1;
  }
  for(iterator = 0; iterator < BIN_LIMIT; iterator++) {
    if(list_number == FREELIST) {
      if(thread_arena.freelist[bin_number][iterator].startAddress != NULL) {
        return iterator;
      }
    } else if(list_number == USEDLIST) {
      if(thread_arena.usedlist[bin_number][iterator].startAddress == NULL) {
        return iterator;
      }
    }
  }
  return -1;
}

void* create_block_and_add_to_usedlist(int size,int iterator) {
  int bin_number = set_list_index(size);
  int usedlist_position = get_first_free_from_list(USEDLIST,size);
  thread_arena.usedlist[bin_number][usedlist_position].startAddress = thread_arena.page[iterator].startAddress;

  thread_arena.page[iterator].size -= size;
  thread_arena.page[iterator].startAddress += size;

  return thread_arena.usedlist[bin_number][usedlist_position].startAddress;
}

/*
Print the statistics for the Malloc arena size,
the total number of bins, including the bin blocks
both used and freed.
*/
void malloc_stats_print() {
  int total_bins = 0;
  int bin_total_blocks[BIN_NUMBER + 1];
  int bin_used_blocks[BIN_NUMBER + 1];
  int bin_free_blocks[BIN_NUMBER];

  int iterator,iterator2;
  for(iterator = 0; iterator < BIN_NUMBER + 1; iterator++) {
    if(iterator < BIN_NUMBER) {
      bin_free_blocks[iterator] = 0;
    }
    bin_total_blocks[iterator] = 0;
    bin_used_blocks[iterator] = 0;
  }

  for(iterator = 0; iterator < BIN_NUMBER; iterator++) {
    bool present = false;
    for(iterator2 = 0; iterator2 < BIN_LIMIT; iterator2++) {
      if(thread_arena.usedlist[iterator][iterator2].startAddress != NULL) {
        bin_used_blocks[iterator]++;
        bin_total_blocks[iterator]++;

        present = true;
      }
      if(thread_arena.freelist[iterator][iterator2].startAddress != NULL) {
        bin_free_blocks[iterator]++;
        bin_total_blocks[iterator]++;

        present = true;
      }
    }
    if(present)	{
      total_bins++;
    }
  }

  bool present = false;
  for(iterator = 0; iterator < BIN_LIMIT; iterator++) {

    if(thread_arena.usedlist[BIN_NUMBER][iterator].startAddress != NULL) {
      bin_used_blocks[BIN_NUMBER]++;
      bin_total_blocks[BIN_NUMBER]++;

      present = true;
    }
  }

  printf("Total size allocated - %d\n", total_size);
  printf("Total number of bins - %d\n", total_bins);

  for(iterator = 0; iterator < BIN_NUMBER + 1; iterator++) {
    printf("Bin %d, total number of blocks - %d\n", iterator, bin_total_blocks[iterator]);
    printf("Bin %d, used blocks - %d\n", iterator, bin_used_blocks[iterator]);

    if(iterator < BIN_NUMBER)	{
      printf("Bin %d, free blocks - %d\n", iterator,bin_free_blocks[iterator]);
      printf("Bin %d, Total allocation requests - %d\n", iterator,alloc_requests[iterator]);
    }

    if(iterator < BIN_NUMBER)	{
      printf("Bin %d, Total free requests - %d\n", iterator,free_requests[iterator]);
    }
  }
}

/*
Test malloc_stats print statistics for ensuring accuracy.
*/
int print_stats() {
  int iterator, iterator2;

  printf("\nFREE-LIST\n");
  for(iterator = 0; iterator < BIN_NUMBER; iterator++) {
    printf("Freelist - %d\n", iterator);
    for(iterator2 = 0; iterator2 < BIN_LIMIT; iterator2++) {
      if(thread_arena.freelist[iterator][iterator2].startAddress == NULL)	{
        continue;
      }
      printf("size - %ld\tstartAddress - %p\n", thread_arena.freelist[iterator][iterator2].size,thread_arena.freelist[iterator][iterator2].startAddress);
    }
  }
  printf("\nUSED-LIST\n");
  for(iterator = 0; iterator < BIN_NUMBER; iterator++) {
    printf("Usedlist - %d\n", iterator);
    for(iterator2 = 0; iterator2 < BIN_LIMIT; iterator2++) {
      if(thread_arena.usedlist[iterator][iterator2].startAddress == NULL)	{
        continue;
      }
      printf("size - %ld\tstartAddress - %p\n", thread_arena.usedlist[iterator][iterator2].size,thread_arena.usedlist[iterator][iterator2].startAddress);
    }
  }
  printf("\nbin\n");
  for(iterator = 0; iterator < MAX_PAGE_LIMIT; iterator++) {
    printf("Bin - %d\n", iterator);
    if(thread_arena.page[iterator].startAddress == NULL)	{
      continue;
    }
    printf("size - %ld\tstartAddress - %p\n", thread_arena.page[iterator].size,thread_arena.page[iterator].startAddress);
  }
  return 0;
}
