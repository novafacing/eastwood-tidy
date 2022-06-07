#include "switch_indentation.h"

/*
 * Good header comment
 */

int main() {
  int a = 10;
  int b = 12;
  switch (a) {
    case 1:
    case 2:
      break;
    case 3:
    case 4:
      a = 10;
      b = 12;
      break;
    case 9: {
      a = 11;
    }
      break;
    case 5:
      break;
    case 6:
    case 7: {
      break;
    }
  }
}