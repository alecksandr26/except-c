/* Simple example with the lib installed, to compile,
   to install just run `make install`
   cc installed.c -ltc
*/

#include <stdio.h>
#include <trycatch.h>

Except someError = {"Some error"},
	anotherError = {"Another error"};

int main(void)
{
	try {
		throw(ExceptBadPtr,
		      "This is a message hello world");
	} catch(someError) {
		puts("Hello, World!");
	} catch(ExceptBadPtr) {
		puts("Bad pointer");
		printf("%s\n", ExceptBadPtr.msg);
	} endtry;
	
	return 0;
}
