/*
 * X, hw5.c, CS 240, Spring 2022
 * Last updated Jan 23, 2022
 */

/* Add any includes here */

#include "max_funcsize_74.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

/* Define your functions here */

#define MONDAY (1)
#define FRIDAY (5)
#define START_TIME (8)
#define END_TIME (21)

teaching_assistant_t read_teaching_assistant(FILE *, int);
int write_teaching_assistant(FILE *, teaching_assistant_t, int);
float compute_percent_by_male(FILE *);
float compute_percent_by_type(FILE *, enum type_t);
float average_rating_by_type(FILE *, enum type_t);
int find_teaching_assistant_by_id(FILE *, int);
int find_teaching_assistant_by_name(FILE *, char *, char *);
int update_teaching_assistant_rating(FILE *, int, float);
int book_office_hours(FILE *, int, int, int);
bool first_name_check(char []);
bool last_name_check(char []);
bool ta_gender_check(teaching_assistant_t);
bool ta_type_check(enum type_t);
bool ta_calendar_check(teaching_assistant_t);













































































































































































































































































/*
 * This function checks that the first name of the ta
 * is not an empty string
 */

bool first_name_check(char first_name[]) {
  if (strlen(first_name) == 0) {
    return false; //empty name string
  }
  return true;
} /* first_name_check() */

/* Remember, you don't need a main function!
 * It is provided by hw5_main.c or  hw5_test.o
 */
