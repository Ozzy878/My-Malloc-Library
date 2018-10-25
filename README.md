MyMalloc Library
================

### Implementing a Malloc Library
My project for creating a malloc library, providing the dynamic memory allocation routines:

>`void *malloc(size_t size);`
>`void free(void *ptr);`
>`void *calloc(size_t nmemb, size_t size);`
>`void *realloc(void *ptr, size_t size);`

**Allocation -** Creating Fixed-size Bins
**Single Freelist -** Frees the blocks instead of keeping separate freelists for each bin size.
**Memory Request -** If no blocks in freelist are available, the kernel will be asked for more memory using `sbrk()`
**Alignment -** Returned address from malloc, calloc and realloc to be aliged at 8-byte boundaries.

#### Printing Malloc Statistics
Calling `malloc_stats();` to print:
>- Total size of arena allocated
- Total number of bins
- For each bin:
    - Total number of blocks
    - Used blocks
    - Free blocks
    - Total allocation requests
    - Total free requests
    
    
- For my Malloc Library Implementation, I used a thread safe 'static __thread' struct arena for executing an arena when a new thread is created/ managed.
Each of the threads created can access memory of another thread if passed a pointer but cannot free the memory.
- Using the `free()` function, serches the used-lists for the pointers to be freed, through the metadata `struct mem_meta instance` within the used-list. When the metadata is found, is then freed and moved to the free-list.
    
To print `malloc_stats()` use `make get-stats` after using the intiial `make` command.

### Build/ Make Project
Use the command `make` and this will build the output files for execution.
The *libmalloc.so* file, is a shared file that is compiled once the `make` command is run.
- It also executes two test files automatically:
    - t-test1.c
    - test1.c
    - test_sbrk


### Running the Test Files
Run `make test` to execute the *test1.c*, *t-test1.c* and *test_sbrk.c* executable files.
**Test the** *Test files* **individually:**
>`make run-test1`
>`make run-t-test1`
>`make run-test_sbrk`

### Clean the Directory
Run `make clean` to clear all of the executable binary files from the directory.


#### _Current known BUGS_
* I have the `malloc_stats();` function in my *malloc.h* header file, but am having trouble outputing all of the requested stats.
* The allocation functions seem to be working fine with some of the testing I have done, but the ouput I receive when calling a `free` request, it seems to output the incorrect stats, whereas else it causes a segment fault error.
