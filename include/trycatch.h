/*!
  @file exception.h
  @brief A simple-based macro interface that helps with dealing with exceptions.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#ifndef TRYCATCH_INCLUDED
#define TRYCATCH_INCLUDED

#include "trycatch/stackjmp.h"

#define E Except
#define F ExceptFrame

enum { EXCEPT_ENTERED = 0, EXCEPT_RAISED, EXCEPT_HANDLED, EXCEPT_FINALIZED };

typedef struct E E;
struct E {
	const char *reason;
};

typedef struct F F;
struct F {
	F *prev;
	JmpBuf contex;
	const char *file;
	int	    line;
	const E	   *exception;
};

#define try                                                               \
	do {                                                              \
		volatile int __except_flag;                                 \
		ExceptFrame  __except_frame; /* Creates the except frame */ \
		/* Link the frames */                                     \
		__except_frame.prev = __except_head;                          \
		__except_head	  = &__except_frame;                        \
		__except_flag	  = stackjmp(&__except_frame.contex);       \
		/* Try something */                                       \
		if (__except_flag == EXCEPT_ENTERED)
#define throw(e) __tc_except_raise(&(e), __FILE__, __LINE__)
#define RE_RAISE \
	__tc_except_raise(__except_frame.exception, __except_frame.file, __except_frame.line)
#define catch(e)							\
	else if (__except_frame.exception == &(e) && (__except_flag = EXCEPT_HANDLED))
#define otherwise else if ((__except_flag = EXCEPT_HANDLED))
#define endtry                                                              \
	;                                                                   \
	if (__except_flag == EXCEPT_ENTERED) __except_head = __except_head->prev; \
	if (__except_flag == EXCEPT_RAISED) RE_RAISE;                         \
	}                                                                   \
	while (0)

extern F *__except_head;
extern void __tc_except_raise(const E *e, const char *file, int line);

#undef E
#undef F
#endif
