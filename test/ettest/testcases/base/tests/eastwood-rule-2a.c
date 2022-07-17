// RUN: %%check_clang_tidy %%s eastwood-rule-2a %%t
/* --------------------- Test Code Style Section II.A
 * Each line must be kept within 80 columns in order to make sure the entire lin
 * e will fit on printouts.  If the line is too long, then it must be broken up
 * into readable segments. The indentation of the code on the following lines ne
 * eds to be at least 2 spaces.
 * ------------------------------------------------------------------------------------------ */
#include "eastwood-rule-2a.h"

#include <stdlib.h>

struct list_node {
  struct list_node *next;
  struct list_node *prev;
  int value;
};

/*
 * foo
 */

int foo(void) {
  // Bad: line length
  int aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa = 10;

  struct list_node * node = NULL;

  // Bad: The expression does not fit on one line and should be indented by 2
  // on subsequent lines
  node->next->next->next->next->next->next->next->next->next->next->next->next
  ->next->next = NULL;
  // Good: The expression does not fit on one line and is indented+2 on
  // subsequent lines
  node->next->next->next->next->next->next->next->next->next->next->next->next->next->next
    ->next
    ->next = NULL;
}