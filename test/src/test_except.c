/* tests for the TRY and EXCEPT modules */
#include "../../include/except.h"

#include <assert.h>
#include <stdio.h>

extern const char *exception;

Except_T SomeError  = INIT_EXCEPT_T("Testing abortion");
Except_T SomeError2 = INIT_EXCEPT_T("Testing abortion 2");
Except_T SomeError3 = INIT_EXCEPT_T("Testing abortion 3");

void test_simple_try_except(void)
{
	int i = 0;

	TRY RAISE(SomeError); /* raise an error */
	EXCEPT (SomeError3) i = 5;
	EXCEPT (SomeError) i++;
	EXCEPT (SomeError2) i = 3;
	ELSE i	     = 2;
	END_TRY;

	assert(i == 1);
}

Except_T NotEnoughMemory		= INIT_EXCEPT_T("Not enough memory"),
	AllocNumShouldBePositive = INIT_EXCEPT_T("AllocNumShouldBePositive");

/* always is going to return null */
void *alloc_will_be_null(int num)
{
	if (num < 1) RAISE(AllocNumShouldBePositive);
	return NULL;
}

/* Simulates an allocation */
void *create_instance_of_something(int n)
{
	void *some_instance;

	if ((some_instance = alloc_will_be_null(n)) == NULL)
		RAISE(NotEnoughMemory); /* throw_except the error */

	return some_instance;
}

void test_simulate_try_except(void)
{
	void *some_instace = NULL;

	TRY some_instace = create_instance_of_something(10);
	EXCEPT (NotEnoughMemory)
	{
		assert(some_instace == NULL);
	}
	END_TRY;
}

void test_multiples_trys(void)
{
	int i = 0;

	TRY RAISE(SomeError);
	EXCEPT (SomeError) i++;
	END_TRY;

	TRY RAISE(SomeError2);
	EXCEPT(SomeError2) i++;
	END_TRY;

	assert(i == 2);
}

void more_try(void)
{
	void *instance = NULL;

	TRY instance = create_instance_of_something(-1);
	EXCEPT (NotEnoughMemory)
	{
		assert(instance == NULL);
	}
	END_TRY;
}

void test_trys_inside_trys(void)
{
	TRY more_try();
	EXCEPT (AllocNumShouldBePositive)
	{
		// printf("%s\n", AllocNumShouldBePositive.reason);
	}
	END_TRY;
}

void test_throw_msg(void)
{
	TRY {
		RAISE(ExceptLogicalError);
	}
	END_TRY;
}

int main(void)
{
	test_simple_try_except();
	test_simulate_try_except();
	test_multiples_trys();
	test_trys_inside_trys();
	
	test_throw_msg();

	return 0;
}
