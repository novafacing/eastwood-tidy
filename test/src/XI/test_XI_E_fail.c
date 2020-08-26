/* --------------------- Test Code Style Section XI.E Fail ---------------------
 * The use of goto is forbidden in this course.
 * -------------------------------------------------------------------------- */
#include "test_XI_E_fail.h"

int test(void) {
    int temp_value = 0;
    temp_value += 10;
    if (temp_value == 0) {
        goto exit; // Fails, goto is forbidden.
    } else {
        return 1;
    }
exit:
    return 0;
}

int main(int argc, char ** argv) {
  printf("{0}\n");
  test();
}
