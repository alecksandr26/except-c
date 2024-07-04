/* Simple example with the lib installed, to compile,
   to install just run `make pkg` and then 'pacman -U pkg'
   cc installed.c -lexcept
*/

#include <stdio.h>
#include <except.h>
#include <string.h>

Except_T someError = INIT_EXCEPT_T("Some error"),
	anotherError = INIT_EXCEPT_T("Another error");

void foo(char *name)
{
	if (strcmp(name, "Erick") == 0)
		RAISE(ExceptBadPtr, "This is a message hello world");
}

int main(void)
{
	char name[] = "Erick";
	int age = 10;
	printf("name: %s, age: %i\n", name, age);
	
	TRY {
		foo(name);
	} EXCEPT(someError) {
		puts("Hello, World!");
	} EXCEPT(ExceptBadArrayNewLength) {
		puts("Bad pointer");
		printf("Msg: %s\n", Except_raise_info.msg);
	} END_TRY;
	
	return 0;
}
