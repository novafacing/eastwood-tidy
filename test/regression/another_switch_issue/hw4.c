/* Homework 4 Main function
 * CS 240 - Fall 2021
 * Last updated Sep 13, 2021
 */

#include "hw4.h"

#include <stdio.h>

/*
 *  This function is used to run the functions implemented in hw3.c.
 *  User input is used to determine which function to run and what
 *  input is passed to it. main() prints the return value of the
 *  selected function.
 */

int main() {
  while (1) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) neg_complex(complex_t)\n"
           "2) inv_complex(complex_t)\n"
           "3) exp_complex(complex_t)\n"
           "4) mandelbrot(complex_t)\n"
           "5) add_complex(complex_t, complex_t)\n"
           "6) sub_complex(complex_t, complex_t)\n"
           "7) mul_complex(complex_t, complex_t)\n"
           "8) div_complex(complex_t, complex_t)\n"
           "9) dot_complex(complex_t, complex_t)\n"
           "\nSelect a function: ");

    int num = 333333;
    scanf("%d", &num);

    if ((num > 9) || (num < 0)) {
      printf("Invalid selection.\n");
      continue;
    }

    if (num == 0) {
      return 0;
    }

    printf("Enter a complex_t number in the form x,y:       ");

    complex_t comp_one = {0.0, 0.0};
    complex_t comp_two = {0.0, 0.0};
    scanf("%lf,%lf", &(comp_one.x), &(comp_one.y));

    if (num > 4) {
      printf("Enter another complex_t number in the form x,y: ");
      scanf("%lf,%lf", &(comp_two.x), &(comp_two.y));
    }

    complex_t result = {0.0, 0.0};
    double double_result = 0.0;
    int int_result = 0;
    switch (num) {
      case 1:
        result = neg_complex(comp_one);
        break;
      case 2:
        result = inv_complex(comp_one);
        break;
      case 3:
        result = exp_complex(comp_one);
        break;
      case 4:
        int_result = mandelbrot(comp_one);
        break;
      case 5:
        result = add_complex(comp_one, comp_two);
        break;
      case 6:
        result = sub_complex(comp_one, comp_two);
        break;
      case 7:
        result = mul_complex(comp_one, comp_two);
        break;
      case 8:
        result = div_complex(comp_one, comp_two);
        break;
      case 9:
        double_result = dot_complex(comp_one, comp_two);
        break;
    }

    if (num == 4) {
      printf("The value returned was %d\n", int_result);
    }
    else if (num == 9) {
      printf("The value returned was %f\n", double_result);
    }
    else {
      printf("The value returned was (%f,%f)\n", result.x, result.y);
    }
  }

  /* not reached. */

  return 0;
} /* main() */
