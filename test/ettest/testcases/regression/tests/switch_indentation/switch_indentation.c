#include "switch_indentation.h"


struct x {
  int a;
  int b;
};

enum y {
  A,
  B,
  C
};

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
    case 8:
      a = 1;
      break;
  }

  switch (10) {
    default:
      break;
  }

  switch (10) {
    case 10:
      break;
  }

  switch (10) {
    default:
      a = 1;
      {
        break;
      }
  }

  switch (10) {
    case 10: {
      a = 1;
    }
      break;
  }


  return 1;
}