// RUN: %%check_clang_tidy %%s eastwood-rule-3c %%t
/* --------------------- Test Code Style Section III.C 
 * One space must be placed after internal semi-colons and commas. 
 * ----------------------------------------------------------------- */
#include "eastwood-rule-3c.h"
#include <stdio.h>

/*
 * foo
 */

int foo(int a,int b) {

  /* Bad: No space after commas */

  printf("%d %d\n",a,b);

  for (int i = 0;i < 10;i++) {
    /* Bad: No space after semicolon */

  }

  for (int i = 0; i < 10; i++) {
    /* Good: space after semicolon */
  }

  return 1;
} /* foo() */

/*
 * bar (good: space after comma)
 */

int bar(int a, int b) {
  return a + b;
} /* bar() */
