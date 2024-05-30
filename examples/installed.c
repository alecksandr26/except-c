/* Simple example with the lib installed, to compile,
   to install just run `make pkg` and then 'pacman -U pkg'
   cc installed.c -lexcept
*/

#include <stdio.h>
#include <except.h>

Except_T someError = INIT_EXCEPT_T("Some error"),
	anotherError = INIT_EXCEPT_T("Another error");

void foo(void)
{
	RAISE(ExceptBadPtr, "This is a message hello world");
}

int main(void)
{
	TRY {
		foo();
	} EXCEPT(someError) {
		puts("Hello, World!");
	} EXCEPT(ExceptBadArrayNewLength) {
		puts("Bad pointer");
		printf("Msg: %s\n", Except_raise_info.msg);
	} END_TRY;
	
	return 0;
}
