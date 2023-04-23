/*!
  @file exception.h
  @brief A simple-based macro interface that helps with dealing with exceptions.
  
  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#define E Except
#define F ExceptFrame

/* Flags to track the status of the exceptions */
enum {
	ExceptEntered = 0,
	ExceptRaised,
	ExceptHandled,
	ExceptFinalized
};

/* The adt for exceptions */
typedef struct E {
	const char *reason;
} E;

/* Adt which contains the info for the exception handling */
typedef struct F F;
struct F {
	F *prev;		/* For linking the frames */
	
	/* Some info for debugin */
	const char *file;
	int line;
	const E *exception;
};


/* The head of the linked exceptions */
extern F *except_head;

/* except_raise: It catches some exception and throw it */
extern void except_raise(const E *e, const char *file, int line);

#define RAISE(e) except_raise(&(e), __FILE__, __LINE__)

/* To re-throw an exception that weren't be able to handled */
#define RE_RAISE except_raise(except_frame.exception, except_frame.file, except_frame.line)


/* Initialize an except_frame for handling an exception */
#define TRY do {				\
	volatile int except_flag;		\
	F except_frame; /* Creates the except_frame */	\
	/* Link the frames */				\
	except_frame.prev = except_head;		\
	except_head = &except_frame;			\
	
	





#undef E
#undef F
#endif
