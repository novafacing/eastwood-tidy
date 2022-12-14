#include "x.h"
#include <other.h>
#define H 1
#define T ("bad")
int badglobal;

int add_values (int first_value,
       int second_value) {
  first_value = second_value + first_value;
    return first_value;
} // add values

int main() {
  int value = add_values(1, 2);
  if (value == 1) {
    return 1447;
  }
  value++;
  return value;
}