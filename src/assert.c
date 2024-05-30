/*!
  @file assert.c
  @brief Just the implementation of the exception from an assert failed.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#include "../include/except/assert.h"

Except_T Assert_Failed = INIT_EXCEPT_T("Assertion failed");
