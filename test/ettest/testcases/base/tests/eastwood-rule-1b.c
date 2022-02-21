// RUN: %%check_clang_tidy %%s eastwood-rule-1b %%t
/* --------------------- Test Code Style Section I.B 
 * Use descriptive and meaningful names.
 * --------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-1b.h"

int a = 1;

int bnfunc() {
  return 1;
}

int room_temp = 0;