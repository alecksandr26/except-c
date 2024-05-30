/*
  cc assert.c -lexcept
 */

#include <except/assert.h>

void bar(void)
{
	assert(0, "This is a message");	
}


void foo(void)
{
	bar();
}

int main(void)
{

	foo();
	
	return 0;
}


