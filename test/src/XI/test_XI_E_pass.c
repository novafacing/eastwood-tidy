/* --------------------- Test Code Style Section XI.E Pass ---------------------
 * The use of goto is forbidden in this course.
 * -------------------------------------------------------------------------- */
#include "test_XI_E_pass.h"

int test(void) {
    return 0; // OK, there is no goto
}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
