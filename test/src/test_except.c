/* tests for the try and except modules */
#include "../../include/except.h"

#include <assert.h>
#include <stdio.h>

extern const char *exception;

Except SomeError  = {"Testing abortion"};
Except SomeError2 = {"Testing abortion 2"};
Except SomeError3 = {"Testing abortion 3"};

void test_simple_try_except(void)
{
	int i = 0;

	try throw_except(SomeError); /* throw_except some error */
	except(SomeError3) i = 5;
	except(SomeError) i++;
	except(SomeError2) i = 3;
	otherwise i	     = 2;
	endtry;

	assert(i == 1);
}

Except NotEnoughMemory		= {"Not enough memory"},
       AllocNumShouldBePositive = {"num Should be positive"};

/* always is going to return null */
void *alloc_will_be_null(int num)
{
	if (num < 1) throw_except(AllocNumShouldBePositive);
	return NULL;
}

/* Simulates an allocation */
void *create_instance_of_something(int n)
{
	void *some_instance;

	if ((some_instance = alloc_will_be_null(n)) == NULL)
		throw_except(NotEnoughMemory); /* throw_except the error */

	return some_instance;
}

void test_simulate_try_except(void)
{
	void *some_instace = NULL;

	try some_instace = create_instance_of_something(10);
	except(NotEnoughMemory)
	{
		assert(some_instace == NULL);
		assert(except_flag == EXCEPT_HANDLED);
	}
	endtry;
}

void test_multiples_trys(void)
{
	int i = 0;

	try throw_except(SomeError);
	except(SomeError) i++;
	endtry;

	try throw_except(SomeError2);
	except(SomeError2) i++;
	endtry;

	assert(i == 2);
}

void more_try(void)
{
	void *instance = NULL;

	try instance = create_instance_of_something(-1);
	except(AllocNumShouldBePositive)
	{
		assert(instance == NULL);
		throw_except(AllocNumShouldBePositive); /* Raise again an error */
	}
	endtry;
}

void test_trys_inside_trys(void)
{
	try more_try();
	except(AllocNumShouldBePositive)
	{
		assert(exception == AllocNumShouldBePositive.reason);
		printf("%s\n", exception);
	}
	endtry;
}

int main(void)
{
	test_simple_try_except();
	test_simulate_try_except();
	test_multiples_trys();
	test_trys_inside_trys();

	return 0;
}
