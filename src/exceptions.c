/*!
  @file exceptions.c
  @brief A few useful general purpuse exceptions.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#include "../include/trycatch/except.h"
#include "../include/trycatch/exceptions.h"

#define E Except

/* Logical exceptions */
E ExceptLogicalError = {"Logical Error"};
E ExceptInvalidArgument = {"Invalid Argument"};
E ExceptDomainError = {"Domain Error"};
E ExceptLengthError = {"Length Error"};
E ExceptOutOfRange = {"Out Of Range"};

/* System error exceptions */
E ExceptRuntimeError = {"Runtime Error"};
E ExceptRangeError = {"Range Error"};
E ExceptOverflowError = {"Overflow Error"};
E ExceptSystemError = {"System Error"};
E ExceptFileSystemError = {"File System Error"};
E ExceptThreadSystemError = {"Thread System Error"};
E ExceptSocketSystemError = {"Socket System Error"};
E ExceptReadSystemError = {"Read System Error"};
E ExceptWriteSystemError = {"Write System Error"};


/* Memory exceptions */
E ExceptBadAlloc = {"Bad Alloc"};
E ExceptBadArrayNewLength = {"Bad Array New Length"};
E ExceptBadPtr = {"Bad Ptr"};


#undef E

