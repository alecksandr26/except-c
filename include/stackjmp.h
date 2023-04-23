/*!
  @file stackjmp.h
  @brief Module that catches the stackjmp of the thread execution of a program.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#ifndef __STACKJMP_H__
#define __STACKJMP_H__

/* The jump buffer */
#define B JmpBuf
typedef struct B {
	/* Alloc memory to be able to catch the process stackjmp execution */
	unsigned long buf[6];
} B;

/* stackjmp: Initialize a stackjmp jump point for a future jumping. */
extern int stackjmp(B *buf);

/* jump: From a initialized buffer makes a jump and set rax register with the val */
extern void jmpback(B *buf, int val);

#undef B
#endif
