// RUN: %%check_clang_tidy %%s eastwood-rule-3d %%t
/* --------------------- Test Code Style Section III.D 
 * #define expressions need to be grouped together and need to be lined up in co
 * lumn 1. They need to have a blank line above and below.
 * -------------------------------------------------------------------- */
#include "eastwood-rule-3d.h"

#define TOP_OF_FILE_DEF (10)
#define TOP_OF_FILE_DEF2 (10)
#define ANOTHER_TOP_OF_FILE_DEF "hello"
  #define BAD_DEF_SHOULD_NOT_BE_INDENTED (12)
int g_global_bad = 0;

#define BAD_DEF_SHOULD_BE_GROUPED (10)
  #define BAD_DEF_SHOULD_NOT_BE_INDENTED_SHOULD_BE_GROUPED (10)

/*
 * foo
 */

int foo() {
#define BAD_DEF_SHOULD_HAVE_PREC_LINE (10)
  #define BAD_DEF_SHOULD_NOT_BE_INDENTED_IN_FUNC_AND_NL (12)
  int i = 0;

#define BAD_DEF_SHOULD_BE_GROUPED_IN_FUNC (10)
  return 0;
} /* foo() */

#define BAD_DEF_SHOULD_BE_IN_FUNC (10)