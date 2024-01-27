/*!
  @file exceptions.h
  @brief A few useful general purpuse exceptions.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#ifndef EXCEPTIONS_INCLUDED
#define EXCEPTIONS_INCLUDED

#include "except.h"

#define E Except

extern E ExceptLogicalError;
extern E ExceptInvalidArgument;
extern E ExceptDomainError;
extern E ExceptLengthError;
extern E ExceptOutOfRange;

extern E ExceptRuntimeError;
extern E ExceptRangeError;
extern E ExceptOverflowError;
extern E ExceptSystemError;
extern E ExceptFileSystemError;
extern E ExceptSocketSystemError;
extern E ExceptReadSystemError;
extern E ExceptWriteSystemError;

extern E ExceptBadAlloc;
extern E ExceptBadArrayNewLength;
extern E ExceptBadPtr;

#undef E

#endif
