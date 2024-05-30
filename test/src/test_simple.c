#include <stdio.h>
#include "../../include/except.h"

struct Person {
	char name[255];
	int age;
};

void test_simple_try(void)
{
	void *ptr = NULL;

	TRY {
		int age = ((struct Person *) ptr)->age;
		printf("name: %s\n", ((struct Person *) ptr)->name);
		printf("age: %i\n", age);
	} EXCEPT(ExceptSigAbrt) puts("The program gets aborted");
	EXCEPT(ExceptSigSegv) puts("ptr is null");
	END_TRY;
}

int main(void)
{
	/* Enable the signal exceptions */
	Except_init_signal_exceptions();
	
	test_simple_try();

	/* Disable the signal exceptions */
	Except_uninit_signal_exceptions();
	
	return 0;
}
