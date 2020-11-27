// RUN: %%check_clang_tidy %%s eastwood-rule-2a %%t
/* --------------------- Test Code Style Section II.A 
 * Each line must be kept within 80 columns in order to make sure the entire lin
 * e will fit on printouts.  If the line is too long, then it must be broken up 
 * into readable segments. The indentation of the code on the following lines ne
 * eds to be at least 2 spaces.
 * ---------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-2a.h"

int foo(void) {
	int a = 10; // Bad: there is a tab here instead of spaces
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here
  int aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa = 10; // Bad: line length
  int b = 10; 
  // Bad, the previous line has a trailing whitespace
}
// Put Something That Fails Here

int bar(void) {
  int a = 10;
}
// OK: Reason
// Some code that doesn't fail here
