#include "../../include/except.h"

#include <stdio.h>

Except_T SomeExcept = INIT_EXCEPT_T("Some Exception");

int main(void)
{
	TRY {
		RAISE(SomeExcept);
	} ELSE {
		puts("Hello there");
	} END_TRY;
	
	return 0;
}





