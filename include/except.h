/*!
  @file except.h
  @brief A simple-based macro interface that helps with dealing with exceptions.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED

#include <stddef.h>
#include <execinfo.h>
#include <setjmp.h>

#define T Except_T

typedef struct T {
	volatile int flag;
	struct T *prev;
	const char *reason;
	jmp_buf env;
} T;

typedef struct Except_Frame {
	volatile int else_status, finally_status;
	struct Except_Frame *prev;
	jmp_buf else_env, finally_env, except_raised_env, env;
} Except_Frame;

typedef struct {
	const char *reason, *file, *func, *msg;
	int line;
} Except_raise_info_T;

enum { Except_entered = 0, Except_running, Except_running_finally };
enum { Except_handled = 0, Except_nhandled, Except_raised };
enum { Except_exist_finally = 0, Except_not_finally};

/* Logical exceptions */
extern T ExceptLogicalError;
extern T ExceptInvalidArgument;
extern T ExceptDomainError;
extern T ExceptLengthError;
extern T ExceptOutOfRange;

/* System error exceptions */
extern T ExceptRuntimeError;
extern T ExceptRangeError;
extern T ExceptOverflowError;
extern T ExceptSystemError;
extern T ExceptFileSystemError;
extern T ExceptThreadSystemError;
extern T ExceptSocketSystemError;
extern T ExceptReadSystemError;
extern T ExceptWriteSystemError;

/* Memory exceptions */
extern T ExceptBadAlloc;
extern T ExceptBadArrayNewLength;
extern T ExceptBadPtr;

/* Signal exceptions */
extern T ExceptSigAbrt;
extern T ExceptFpe;
extern T ExceptSigIll;
extern T ExceptSigSegv;
extern T ExceptSigTerm;
extern T ExceptSigSys;

extern Except_Frame *Except_frame_stack;
extern Except_raise_info_T Except_raise_info;

extern void Except_init_signal_exceptions(void);
extern void Except_uninit_signal_exceptions(void);
extern void Except_link_handled(T *e);
extern void Except_unlink_handlers(void);
extern void Except_unlink_frame_stack(const Except_Frame *actual_frame);
extern void Except_raise(T *e, const char *file, const char *func, int line, const char *msg);
extern void Except_abort_already_handled(T *e, const char *file, const char *func, int line);

#define MAX_BACKTRACE_NAME_SIZE 256
#define MAX_BACKTRACE_LINES 64

extern int Except_backtrace_size;
extern void *Except_backtrace_array[MAX_BACKTRACE_LINES];
extern char **Except_backtrace_strings;

#ifndef __GET_FIRST
#define __GET_FIRST(X, ...) X
#endif

#define INIT_EXCEPT_T(r) {.reason = r, .flag = Except_nhandled, .prev = 0}

#define RAISE(e, ...) do  {							\
		Except_backtrace_size = backtrace(Except_backtrace_array, MAX_BACKTRACE_LINES);	\
		Except_backtrace_strings = backtrace_symbols(Except_backtrace_array, Except_backtrace_size); \
		Except_raise(&(e), __FILE__, __func__, __LINE__,	\
			     __GET_FIRST(__VA_ARGS__ __VA_OPT__(,) 0));	\
	} while (0)

#define TRY do {							\
	volatile int Except_flag;					\
	Except_Frame Except_frame = { .prev = NULL, .else_status = Except_nhandled, \
				      .finally_status = Except_nhandled}; \
	Except_frame.prev = Except_frame_stack;				\
	Except_frame_stack = &Except_frame;				\
	if ((Except_flag = setjmp(Except_frame.env)) == Except_running)

#define EXCEPT(e)						\
	else if ((Except_flag == Except_entered				\
		  && (Except_link_handled(&e), 0))			\
		 || (e.flag == Except_handled && (Except_abort_already_handled(&(e), __FILE__, \
									       __func__, __LINE__), 0)) \
		 || (e.flag = setjmp(e.env)) == Except_raised)

#define ELSE						\
	else if ((Except_frame.else_status = setjmp(Except_frame.else_env)) == Except_raised)

#define FINALLY					\
	if (Except_flag == Except_running				\
	    || (Except_frame.finally_status = setjmp(Except_frame.finally_env)) == Except_running_finally)

#define END_TRY								\
	if (Except_frame.finally_status == Except_running_finally)	\
		longjmp(Except_frame.except_raised_env,			\
			Except_running_finally);			\
	if (Except_flag == Except_entered) longjmp(Except_frame.env, Except_running); \
	Except_unlink_handlers();					\
	Except_unlink_frame_stack(&Except_frame);			\
	} while (0)

#undef T

#endif // EXCEPT_INCLUDED
