#include "x.h"

int badglobal;

/*
 * add_values: add two values and return the result
 */

int add_values (int first_value,
       int second_value) {
  first_value = second_value + first_value;
    return first_value;
} /* add_values() */

/*
 * main: demonstration main function
 */

int main() {
  int value = add_values(1, 2);
  if (value == 1) {
    goto end;
  }
  value++;
  end:
  return value;
}