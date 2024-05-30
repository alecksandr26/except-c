/* To Compile:
   cc optimize.c ../lib/libexcept.a  -O2 -DNDEBUG -fno-stack-protector -z execstack -no-pie
 */
#include <stdio.h>
#include <assert.h>
#include "../include/except.h"

Except_T SomeError = INIT_EXCEPT_T("Some error");

int main(void)
{
	int i = 0;
	
	TRY {
		i++;
		printf("i = %i\n", i);
		RAISE(SomeError);
	} EXCEPT(SomeError) {
		i++;
		printf("i = %i\n", i);
	} END_TRY;

	assert(i == 1);
	
	return 0;
}
