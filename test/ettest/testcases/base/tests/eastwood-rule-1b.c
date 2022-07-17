// RUN: %%check_clang_tidy %%s eastwood-rule-1b %%t
/* --------------------- Test Code Style Section I.B
 * Use descriptive and meaningful names.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-1b.h"

// Ok: should print out when dumping variable names
int g_a = 1;

// Ok: should print out when dumping variable names
int g_room_temp = 0;

/*
 * bnfunc
 */

int bnfunc() {
  return 1;
} /* bnfunc() */
