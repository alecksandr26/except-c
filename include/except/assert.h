/*!
  @file assert.h
  @brief An override of the default 'assert' in C. Now an Assertion Failed will be consider as an Exception.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#ifndef ASSERT_INCLUDED
#define ASSERT_INCLUDED

#include "../except.h"

#ifdef assert
#undef assert
#endif

#ifdef NDEBUG
#define assert(e, ...) ((void)0)
#else
extern Except_T Assert_Failed;
#define assert(e, ...) do {						\
		if (!e)	{						\
			RAISE(Assert_Failed, __GET_FIRST(__VA_ARGS__ __VA_OPT__(,) 0));	\
		}							\
	} while (0)

#endif

#endif // ASSERT_INCLUDED
