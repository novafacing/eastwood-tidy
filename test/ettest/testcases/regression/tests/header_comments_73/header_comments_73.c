/* X, hw5.c, CS 240, Spring 2022
 * Last updated Jan 23, 2022
 */

/* Add any includes here */

#include "header_comments_73.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* CONSTANTs */

#define TA_STRUCT_SIZE (sizeof(teaching_assistant_t))

/* Define your functions here */

/*
 * function to check that the given file pointer isn't null
 */

bool file_not_null_check(FILE *fp) {
  assert(fp != NULL);
  return true;

} /* file_not_null_check() */

/*
 * function that gets the given teaching assistant at a certain position in
 * in the binary file
 */

teaching_assistant_t read_teaching_assistant(FILE *fp, int record_num) {
  file_not_null_check(fp);
  record_num_in_range(record_num);

  fseek(fp, 0, SEEK_END);
  int file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  fseek(fp, record_num * TA_STRUCT_SIZE, SEEK_SET);

  /* check if the record position goes beyond the file length
   * if so give an error
   */

  if (ftell(fp) >= file_size) {
    return BAD_TA;
  }
  teaching_assistant_t found_ta = {};
  int result = fread(&found_ta, TA_STRUCT_SIZE, 1, fp);

  if (result == 1) {
    return found_ta;
  }
  return BAD_TA;

} /* read_teaching_assistant() */
