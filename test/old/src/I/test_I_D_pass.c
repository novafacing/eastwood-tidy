/* --------------------- Test Code Style Section I.D Pass ----------------------
 * All global variables must be started with prefix 'g_'.Declarations/definition
 * s should be at the top of the file.
 * -------------------------------------------------------------------------- */
#include "test_I_D_pass.h"

int g_temperature = 0; // OK because starts with g_ and is at top of file

int test(void) {
    return 0;
}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
