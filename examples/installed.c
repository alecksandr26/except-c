/* Simple example with the lib installed, to compile,
   to install just run `make install`
   cc installed.c -lexcept 
*/


#include <except.h>
#include <stdio.h>

Except someError = {"Some error"};

int main(void)
{
	try
		raise(someError);
	except(someError)
		puts("Testing the installed lib");
	endtry;
	
	return 0;
}
