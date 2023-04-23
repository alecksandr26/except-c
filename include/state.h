/*!
  @file state.h
  @brief Module that catches the state of the thread execution of a program.
  
  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/


#ifndef __STATE_H__
#define __STATE_H__

/* The jump buffer */
#define B JmpBuf
typedef struct B {
	/* Alloc memory to be able to catch the process state execution */
	unsigned long buf[6];
} B;

/* setjmp: Initialize a state jump point for a future jumping. */
extern int setjmp(B *buf);

#undef B
#endif
