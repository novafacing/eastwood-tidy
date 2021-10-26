// RUN: %%check_clang_tidy %%s eastwood-rule-3a %%t
/* --------------------- Test Code Style Section III.A 
 * One space must be placed after all structure control, and flow commands. One 
 * space must also be present between the closing parenthesis and opening brace.
 * ----------------------------------------------------------------------------------------------------- */
#include "eastwood-rule-3a.h"

int foo(void) {
  while (((1))){
    int a = 10;
  }

  while (((1)))  {
    int a = 10;
  }

  while(((1))) {
    int a = 10;
  }

  for (int i = 0; i < 10; i++){
  }

  for (int i = 0; i < 10; i++)  {
  }

  for(int i = 0; i < 10; i++) {
  }

  if (10 == 10){
  }

  if (10 == 10)  {
  }

  if(10 == 10) {
  }

  switch (10){
    default:
      break;
  }

  switch (10)  {
    default:
      break;
  }

  switch(10) {
    default:
      break;
  }


}

// Put Something That Fails Here
// CHECK-MESSAGES: :[[@LINE-1]]:[column goes here]:  Failure Message Goes Here

int bar(void) {
  while (1) {
    // OK: One space.

  }

  if (10 == 10) {
  }

  for (int i = 0; i < 10; i++) {
  }

  switch (10) {
    default:
      break;
  }
}

// OK: Reason
// Some code that doesn't fail here
