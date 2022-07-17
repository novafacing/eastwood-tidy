// RUN: %%check_clang_tidy %%s eastwood-rule-11a %%t
/* --------------------- Test Code Style Section XI.A
 * Do not use tabs for indentation.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-11a.h"


int main() {
  // Bad: there is a tab here instead of spaces
	int a = 10;
} /* main() */
