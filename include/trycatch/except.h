/*!
  @file except.h
  @brief The Definition of the exception structure.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/


#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED

#define E Except

enum { EXCEPT_ENTERED = 0, EXCEPT_RAISED, EXCEPT_HANDLED, EXCEPT_FINALIZED };

typedef struct E E;
struct E {
	const char *reason, *msg;
};

#undef E
#endif

