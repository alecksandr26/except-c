/*!
  @file setjmp.h
  @brief Module that catches the setjmp of the thread execution of a program.
  
  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/


#ifndef __SETJMP_H__
#define __SETJMP_H__

/* The jump buffer */
#define B JmpBuf
typedef struct B {
	/* Alloc memory to be able to catch the process setjmp execution */
	unsigned long buf[6];
} B;

/* setjmp: Initialize a setjmp jump point for a future jumping. */
extern int setjmp(B *buf);

/* longjmp: From a initialized buffer makes a jump and setting rax register with the val */
extern void longjmp(B *buf, int val);

#undef B
#endif
