/*!
  @file exception.h
  @brief A simple-based macro interface that helps with dealing with exceptions.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#ifndef TC_INCLUDED
#define TC_INCLUDED

#include "tc/stackjmp.h"

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
		volatile int except_flag;                                 \
		ExceptFrame  except_frame; /* Creates the except_frame */ \
		/* Link the frames */                                     \
		except_frame.prev = except_head;                          \
		except_head	  = &except_frame;                        \
		except_flag	  = stackjmp(&except_frame.contex);       \
		/* Try something */                                       \
		if (except_flag == EXCEPT_ENTERED)
#define throw(e) except_raise(&(e), __FILE__, __LINE__)
#define RE_RAISE \
	except_raise(except_frame.exception, except_frame.file, except_frame.line)
#define catch(e)							\
	else if (except_frame.exception == &(e) && (except_flag = EXCEPT_HANDLED))
#define otherwise else if ((except_flag = EXCEPT_HANDLED))
#define endtry                                                              \
	;                                                                   \
	if (except_flag == EXCEPT_ENTERED) except_head = except_head->prev; \
	if (except_flag == EXCEPT_RAISED) RE_RAISE;                         \
	}                                                                   \
	while (0)

extern F *except_head;
extern void except_raise(const E *e, const char *file, int line);

#undef E
#undef F
#endif
