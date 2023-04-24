/* Simple example with the lib installed, to compile,
   to install just run `make install`
   cc installed.c -lexcept 
*/


#include <stdio.h>
#include <except.h>

Except someError = {"Some error"},
	anotherError = {"Another error"};

int main(void)
{
	try
		raise(someError);
	except(someError)
		puts("Hello, World!");
	endtry;
	
	return 0;
}
