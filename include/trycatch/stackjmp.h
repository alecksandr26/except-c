/*!
  @file stackjmp.h
  @brief Module that catches the stackjmp of the thread execution of a program.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#ifndef STACKJMP_INCLUDED
#define STACKJMP_INCLUDED

#define B JmpBuf

typedef struct B B;
struct B {
	unsigned long buf[6];
};

extern int stackjmp(B *buf);
extern void jmpback(B *buf, int val);

#undef B
#endif
