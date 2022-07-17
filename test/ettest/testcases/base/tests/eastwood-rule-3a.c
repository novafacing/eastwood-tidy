// RUN: %%check_clang_tidy %%s eastwood-rule-3a %%t
/* --------------------- Test Code Style Section III.A 
 * One space must be placed after all structure control, and flow commands. One 
 * space must also be present between the closing parenthesis and opening brace.
 * ----------------------------------------------------------------- */
#include "eastwood-rule-3a.h"

/*
 * foo
 */

int foo(void) {

  // Bad: No space before brace
  while (((1))){
    int a = 10;
  }

  // Bad: Must be exactly one space
  while (((1)))  {
    int a = 10;
  }

  // Bad: No space after control flow
  while(((1))) {
    int a = 10;
  }
  // Bad: no space before brace
  for (int i = 0; i < 10; i++){
  }

  // Bad: Must be exactly one space
  for (int i = 0; i < 10; i++)  {
  }

  // Bad: No space after control flow
  for(int i = 0; i < 10; i++) {
  }

  // Bad: no space before brace
  if (10 == 10){
  }

  // Bad: Must be exactly one space
  if (10 == 10)  {
  }

  // Bad: No space after control flow
  if(10 == 10) {
  }
  else{

  }

  // Bad: no space before brace
  switch (10){
    default:
      break;
  }

  // Bad: Must be exactly one space
  switch (10)  {
    default:
      break;
  }

  // Bad: No space after control flow stmt
  switch(10) {
    default:
      break;
  }


} /* foo() */

/*
 * bar
 */

int bar(void) {
  // Good: One space
  while (1) {

  }

  // Good: One space
  if (10 == 10) {
  }
  else {
    int a = 10;
  }

  // Good: One space
  for (int i = 0; i < 10; i++) {
  }

  // Good: One space
  switch (10) {
    default:
      break;
  }
} /* bar() */
