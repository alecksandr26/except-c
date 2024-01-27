
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

#include "../include/trycatch.h"

#define MAX_ALLOC_SIZE 1024

void *struct_allocator(size_t n, size_t struct_size)
{
	if (n == 0 && MAX_ALLOC_SIZE < n)
		throw(ExceptRangeError,
		      "struct_allocator: invalid number of elements to be alloced");
	if (struct_size == 0)
		throw(ExceptInvalidArgument,
		      "struct_allocator: invalid element size to be alloced");
	
	return malloc(n * struct_size);
}

int main(void)
{
	void *ptr;
	try {
		ptr = struct_allocator(0, 0);
	} catch(ExceptRangeError) {
		puts("Catching the excpetion");
		printf("the msg: %s\n", );
	} endtry;
	
	return 0;
}




