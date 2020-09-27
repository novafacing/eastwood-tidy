/* --------------------- Test Code Style Section I.A Fail ----------------------
 * Variable names should be in all lowercase. If the name is composed of more th
 * an one word, then underscores must be used to separate them.
 * -------------------------------------------------------------------------- */
#include "test_I_A_fail.h"

int test(void) {
    int roomTemperature = 0; // Fails because there is a capital letter
    int roomtemperature = 0; // Fails because there is no underscore (hard)
    return 1;
}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
