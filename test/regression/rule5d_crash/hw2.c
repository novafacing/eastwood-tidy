/* Name, hw2.c, CS 240, Spring 2022
 * Last updated Jan 16, 2022
 */

/* Add any includes here */

#include "hw2.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Define your functions here */

struct flight_info{
  int month;
  int day;
  int year;
  char from_city[MAX_CITY_LEN];
  char to_city[MAX_CITY_LEN];
  int number_people;
  int ticket_cost;
};

struct flight_info *list_flights;

int list_size;
void debug_print() {
  for(int i = 0; i < list_size; i++){
    struct flight_info ce = list_flights[i];
    printf("%d/%d/%d|%s|%s|%d|%d\n",ce.month,ce.day,ce.year,ce.from_city,ce.to_city,ce.number_people,ce.ticket_cost);
  }
}

/*
//temporary main for testing
int main(int argc, char *argv[]){
  if(argc != 2) printf("bad arg\n");
  printf("status: %d\n",parse_file(argv[1]));
  debug_print();
}
*/

/* Remember, you don't need a main function!
 * It is provided by hw2_main.c or  hw2_test.o
 */