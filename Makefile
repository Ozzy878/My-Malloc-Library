# Mozilla clang - format
ifdef CHECK_TYPE #{
  ifneq ($(flavor var_type),recursive) #{
    $(warning var should be expandable but detected var_type=$(flavor var_type))
  endif #}
endif #}


all: check

default: check

clean:
	rm -rf libmalloc.so malloc.o malloc_stats.o free.o calloc.o realloc.o test1.o test1 t-test1.o t-test1 test_sbrk test_sbrk.o stats

# Library to be created for compiling all of the malloc library files.
libmalloc.so: malloc.o malloc_stats.o free.o calloc.o realloc.o malloc.h stats
	gcc ${CFLAGS} -shared -o libmalloc.so malloc.o malloc_stats.o free.o calloc.o realloc.o

malloc.o: 		malloc.c
	gcc -fmax-errors=10 malloc.c malloc_stats.c malloc.h -fPIC -shared -o malloc.o

malloc_stats.o: malloc_stats.c
	gcc -fmax-errors=10 malloc_stats.c malloc.h -fPIC -shared -o malloc_stats.o

free.o: 		free.c
	gcc -fmax-errors=10 free.c -fPIC -shared -o free.o

calloc.o: 		calloc.c
	gcc -fmax-errors=10 calloc.c -fPIC -shared -o calloc.o

realloc.o: 		realloc.c
	gcc -fmax-errors=10 realloc.c -fPIC -shared -o realloc.o

test1.o: 		test1.c
	gcc -c -pthread -fPIC -o test1.o test1.c

test1:			test1.o
	gcc -g -pthread -o test1 test1.o

t-test1.o: 		t-test1.c
	gcc -c -pthread -fPIC -o t-test1.o t-test1.c

t-test1:		t-test1.o
	gcc -g -pthread -o t-test1 t-test1.o

test_sbrk.o: 		test_sbrk.c
	gcc -c -fPIC -o test_sbrk.o test_sbrk.c

test_sbrk:		test_sbrk.o
	gcc -g -o test_sbrk test_sbrk.o

# Main function in malloc.c to run the get_stats() function
stats: malloc.c malloc_stats.c malloc.h
	gcc -o stats malloc.c malloc_stats.c malloc.h

test:
	./test1
	./t-test1
	./test_sbrk

run-test1:		test1
	./test1

run-t-test1: 	t-test1
	./t-test1

run-test_sbrk: test_sbrk
	./test_sbrk

gdb:
	gdb --args ./t-test1

check:			clean libmalloc.so test1 t-test1 test_sbrk
	LD_PRELOAD="`pwd`/libmalloc.so" ./t-test1
	LD_PRELOAD="`pwd`/malloc.o `pwd`/malloc_stats.o `pwd`/free.o `pwd`/calloc.o `pwd`/realloc.o" ./test1

get-stats:
	./stats


# done
