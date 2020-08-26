/* --------------------- Test Code Style Section I.A Pass ----------------------
 * Variable names should be in all lowercase. If the name is composed of more th
 * an one word, then underscores must be used to separate them.
 * -------------------------------------------------------------------------- */
#include "test_I_A_pass.h"

int test(void) {
    int temperature = 0;      // OK because it is a single word, lowercase
    int room_temperature = 0; // OK because it is two words but separated by an underscore
    return 0;
}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
