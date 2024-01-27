/*!
  @file exception.c
  @brief Some of the implementations of the module.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#include "../include/trycatch.h"

#include "../include/trycatch/except.h"
#include "../include/trycatch/exceptions.h"
#include "trycatch/stackjmp.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define F ExceptFrame
#define E Except

F	   *__except_head;
const char *exception;

void __tc_except_raise(const E *e, const char *file, int line, const char *func)
{
	assert(e != NULL && file != NULL && line > 0 && func != NULL &&
	       "Can't receive null pointers");

	F *frame = __except_head;

	if (frame == NULL) {
		fprintf(stderr, "Traceback...\n");
		fprintf(stderr, "\tFile \"%s\", line %i, raised in func \"%s\"\n", file,
			line, func);
		fprintf(stderr, "UncaughtException:");

		if (e->reason)
			fprintf(stderr, "\t \"%s\"%s", e->reason, (e->msg) ? "," : "\n");
		if (e->msg) fprintf(stderr, "\t \"%s\"\n", e->msg);

		fprintf(stderr, "\nAborting....\n");
		fflush(stderr);

#ifndef NDEBUG
		abort();
#else
		fprintf(stdout, "Testing the abortion of the  throw exiting success");
		exit(EXIT_SUCCESS);
#endif
	}

	exception	 = e->reason;
	frame->exception = e;
	frame->file	 = file;
	frame->line	 = line;
	frame->func	 = func;
	__except_head	 = __except_head->prev;

	jmpback(&frame->contex, EXCEPT_RAISED);
}

#undef E
#undef F
