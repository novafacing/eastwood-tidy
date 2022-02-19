/* HW1 solution for test only - Fall 2021 */

#include "hw1.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* int encode(int, int, int) here 
 * some definition
 */

int my_encode(int n, int l, int r) {
  if ((l < 0) || (r < 0) || (l > ARRAY_SIZE) || (r > ARRAY_SIZE)) {
    return OUT_OF_BOUNDS;
  }
  if ((n < 0) || (l > r)) {
    return UNEXPECTED_INPUT;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    g_cip[i] = g_msg[i];
  }
  for (int i = l; i < r; i++) {
    if (((g_msg[i] - 'a') >= 0) && ((g_msg[i] - 'a') < 26)) {
      g_cip[i] = (g_msg[i] - 'a' + n) % 26 + 'a';
    }
    if (((g_msg[i] - 'A') >= 0) && ((g_msg[i] - 'A') < 26)) {
      g_cip[i] = (g_msg[i] - 'A' + n) % 26 + 'A';
    }
  }
  return SUCCESS;
} /* my_encode() */

/* int checksum() here 
 * some definition
 */

int my_checksum(void) {
  int sum = 0;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    sum = sum ^ g_msg[i];
  }
  return sum;
} /* my_checksum() */
