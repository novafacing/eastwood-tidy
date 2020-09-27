/* --------------------- Test Code Style Section I.D Fail ----------------------
 * All global variables must be started with prefix 'g_'.Declarations/definition
 * s should be at the top of the file.
 * -------------------------------------------------------------------------- */
#include "test_I_D_fail.h"

int temperature = 0; // Fails because global does not start with g

int test(void) {
    return 1;
}

int temp_of_room = 0; // Fails because global is not declared at the top of the file

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
