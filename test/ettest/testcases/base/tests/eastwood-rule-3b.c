// RUN: %%check_clang_tidy %%s eastwood-rule-3b %%t
/* --------------------- Test Code Style Section III.B
 * One space must be placed before and after all logical, and  arithmetic operat
 * ors; except for unary and data reference  operators (i.e. [], ., &, *, ->).
 * ----------------------------------------------------------------- */
#include "eastwood-rule-3b.h"
#include <stdlib.h>

/*
 * bad
 */

int bad(){
  // Bad: no spacing
  int a = 10+4;
  int b = 10 +4;
  int c = 10+ 4;

  // Bad: no spacing
  if (a==14) {
    // Bad: no spacing
    if ((a==14)&&(10==10)) {
      // Bad: no spacing
      int i = (5,10);
    }
  }
  return a;
} /* bad() */

/*
 * good
 */

int good() {
  // OK: spacing
  int a = 10 + 4;
  // OK: spacing
  if (a == 14) {
    // OK: spacing
    if ((a == 14) && (10 == 10)) {
      // OK: spacing
      int i = (5, 10);
    }
  }
  struct x {
    int y;
  };
  struct x * z = (struct x*)malloc(sizeof(struct x));
  z->y = 0;
  return a;
} /* good() */
