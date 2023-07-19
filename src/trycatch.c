/*!
  @file exception.c
  @brief Some of the implementations of the module.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/trycatch.h"

#define F ExceptFrame
#define E Except

F	   *__except_head;
const char *exception;

void __tc_except_raise(const E *e, const char *file, int line)
{
	assert(e != NULL);

	F *frame = __except_head;

	if (frame == NULL) { 
		fprintf(stderr, "Uncaught exception: ");
		if (e->reason) fprintf(stderr, "%s:", e->reason);
		else fprintf(stderr, "at 0x%p:", (void *) e);
		if (file && line > 0) fprintf(stderr, " raised at %s:%i\n", file, line);
		
		fprintf(stderr, "Aborting....\n");
		fflush(stderr);
		abort();
	}

	exception	 = e->reason;
	frame->exception = e;
	frame->file	 = file;
	frame->line	 = line;
	__except_head = __except_head->prev;
	
	jmpback(&frame->contex, EXCEPT_RAISED);
}

#undef E
#undef F
