#include <stdio.h>
#include "../../include/except.h"

Except_T SomeExcept = INIT_EXCEPT_T("Some Exception");


void bar(void)
{
	TRY {
		if (1) {
			RAISE(SomeExcept);
		}
	} FINALLY puts("Excecuting finally 3");
	END_TRY;
}

void foo(void)
{
	TRY bar();
	FINALLY puts("Executing finally 2");
	END_TRY;
}

int main(void)
{

	TRY foo();
	FINALLY puts("Executing finally");
	END_TRY;
	
	return 0;
}

