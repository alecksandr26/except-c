/*!
  @file exception.c
  @brief Some of the implementations of the module.
  
  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../include/except.h"

#define F ExceptFrame
#define E Except

F *except_head;
const char *exception;

/* To raise the exception aborting the program */
void except_raise(const E *e, const char *file, int line)
{
	assert(e != NULL);


	F *frame = except_head;

	/* Check if there is a frame for exception handling */
	if (frame == NULL) { /* Means that there isn't an exception frame handling */
		fprintf(stderr, "Uncaught exception: ");

		if (e->reason)
			fprintf(stderr, "%s:", e->reason);
		else
			fprintf(stderr, "at 0x%p:", (void *) e);

		if (file && line > 0)
			fprintf(stderr, " raised at %s:%i\n", file, line);
		
		/* Aborts the program */
		fprintf(stderr, "Aborting....\n");
		fflush(stderr);
		abort();
	}

	exception = e->reason;
	frame->exception = e;
	frame->file = file;
	frame->line = line;


	/* Pop from the stack */
	except_head = except_head->prev;
	
	/* Search for handlers */
	longjmp(&frame->contex, EXCEPT_RAISED);
}

#undef E
#undef F
