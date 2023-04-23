/* To Compile:
   cc optimize.c ../lib/libexcept.a  -O2 -DNDEBUG -fno-stack-protector -z execstack -no-pie
 */
#include <stdio.h>
#include <assert.h>
#include "../include/except.h"

Except SomeError = {"Some error"};

int main(void)
{
	int i = 0;
	
	try {
		i++;
		printf("i = %i\n", i);
		raise(SomeError);
	} except(SomeError) {
		i++;
		printf("i = %i\n", i);
	} endtry;

	assert(i == 1);

	return 0;
}
