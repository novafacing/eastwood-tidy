/* Pete R. Purdue, Boilermaker, hwx.c CS 240, Fall 2018  */
/*           Last updated  September 19, 2018            */

/*  Rename this file hwx.c and then use the commmand
 *  gcc -Wall -Werror -std=c99 hwx.c -lm to compile the program
 *  use a.out to run the program
 */
#include "style_sample.h"

#include <stdio.h>
#include <math.h>
#include <assert.h>

#define OK (0)
#define ERROR (1)
#define MAX_N (30)

/* Function prototypes */

void exp_series(double value, int num_terms);
double power(double value, int n);
double factorial(int n);

/*
 *  Homework x
 *
 *  Pete R. Purdue (boilermaker)
 *
 *  Purpose: The following program calculates the value
 *  of exp(x), for some user supplied x, using the Taylor
 *  series expansion for exp(x).  A table is printed that
 *  shows the value of the series after each new term is added.
 *  For comparison, the value returned by the standart math
 *  library's exp() function is also included in the table.
 *
 *   INPUT: x - the argument of the exp function
 *          num_terms - the number of terms
 *                      to be used in the series
 *
 *          note:  The number of terms should be limited to 30.
 *
 *  OUTPUT: A table of convergence with num_terms entries
 *          All prompts and error messages are to be sent to
 *          standard error (stderr).
 */

int main(void) {
  double x = 0.0;       /* The input value used in exp() */

  int num_terms = 0;    /* the number of terms used in the series */

  printf("\nWelcome to the exp (x) Taylor series calculator\n");
  printf("************************************************\n\n");

  /* Prompt for input parameter x */

  fprintf(stderr, "Please enter a value x\n");
  fprintf(stderr, "followed by a Return or and Enter\n");
  fprintf(stderr, "x: ");
  fflush(stderr);

  if (scanf("%lf", &x) != 1) {
    fprintf(stderr, "Scan for x failed\n");
    fprintf(stderr, "Terminating the program\n");
    exit(ERROR);
  }

  /* Prompt for number of terms */

  fprintf(stderr, "Please enter the total number of terms n\n");
  fprintf(stderr, "followed by a Return or and Enter\n");
  fprintf(stderr, "     0 < n <= %d\n", MAX_N);
  fprintf(stderr, "n: ");
  fflush(stderr);

  if (scanf("%d", &num_terms) != 1) {
    fprintf(stderr, "Scan for n failed\n");
    fprintf(stderr, "Terminating the program\n");
    exit(ERROR);
  }

  if ((num_terms <= 0) || (num_terms > MAX_N)) {
    fprintf(stderr, "invalid number of terms entered\n");
    fprintf(stderr, "   0 < n <= %d\n", MAX_N);
    fprintf(stderr, "Terminating the program\n");
    exit(ERROR);
  }

  /* Print table and data */

  printf("\n");
  printf("              ");
  printf("Taylor Series Approximation Table for exp(x)\n");
  printf("              ");
  printf("********************************************\n\n");
  printf("input value:  x = %g \n", x);
  printf("Total number of terms in the series:  n = %d \n", num_terms);
  printf("  no. of Terms          ");
  printf("Series value                  ");
  printf("Actual exp value\n");
  printf(" **************        **************");
  printf("                ******************\n");
  fflush(stdout);

  exp_series(x, num_terms);

  return OK;

} /* main() */

/*
 *  This function calculates exp (x) by using the
 *  Taylor series expansion for exp (x).
 *
 *  Computes series iteratively by generating a table
 *  of values that includes the number of terms for each
 *  cycle, the value of the series for that
 *  cycle, and the math library value for exp.
 *
 *  note:  0 < number of terms <= MAX_N
 */

void exp_series(double x, int num_terms) {
  double series_value = 0.0;  /* Series value of exp (x) */
  double exp_value = 0.0;     /* value from exp (x) */

  /* Range check num_terms */

  assert((num_terms > 0) && (num_terms <= MAX_N));

  exp_value = exp(x);

  for (int i = 0; i < num_terms; ++i) {
    series_value += power(x, i) / factorial(i);
    printf("%8d %25f %30f\n", i + 1 , series_value, exp_value);
  }

  return;

} /* exp_series() */

/*
 *  This function calculates value to the nth power.
 *
 *  value to the nth power is calculated by using a
 *  loop to multiply value times itself n times.
 */

double power(double value, int n) {
  double result = 1.0;   /* Must be initialize to 1 for */
                         /* the multiplcation to work   */

  /* Range check n */

  assert(n >= 0);

  for (int i = 1; i <= n; ++i) {
    result *= value;
  }

  return (result);

} /* power() */

/*
 *  The following function is used to calculate n!
 *
 *  n! is calculated by using an incrementing loop
 *  which multiplies the integers 1 to n.
 *
 *  note:  n >= 0
 */

double factorial(int n) {
  double result = 1.0;   /* Must be initialize to 1 for */
                         /* the multiplcation to work   */

  /* Range check n */

  assert(n >= 0);

  for (int i = 1; i <= n; ++i) {
    result *= i;
  }

  return (result);

} /* factorial() */
