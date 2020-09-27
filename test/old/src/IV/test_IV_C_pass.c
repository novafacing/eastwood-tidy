/* --------------------- Test Code Style Section IV.C Pass ---------------------
 * Do while loops need to be indented with the while on the same line as the clo
 * sing brace.
 * -------------------------------------------------------------------------- */
#include "test_IV_C_pass.h"

int test(void) {
    int temp_var = 0;
    do {
        temp_var++;
    } while (temp_var <= 100); // OK because while is on same line as close brace
    return temp_var;
}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
