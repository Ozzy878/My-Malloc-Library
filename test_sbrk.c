#include <unistd.h>
#include <stdio.h>

void *my_malloc(size_t size) {
	void *p1;
	void *p2;
	void *p3;
	
	p1 = sbrk(4096);
	if (p1 == (void*)-1) {
		return NULL;
	}
	p2 = sbrk(0);
	printf("Allocated 4096: 0x%p ; p2 : 0x%p \n", p1, p2);

	p3 = sbrk(size);
	if (p3 == (void*)-1) {

		return NULL;
	}
	printf("Allocated %ld : 0x%p \n", size, p3);
	return p3;
}

int main() {
	int *p;

	p = my_malloc(5);
	printf("\nMy p: 0x%p \n", p);
}
