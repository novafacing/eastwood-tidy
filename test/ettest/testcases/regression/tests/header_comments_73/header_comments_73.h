#ifndef HEADER_COMMENTS_73_H
#define HEADER_COMMENTS_73_H

#include <stdio.h>

/* Constant definitions */

#define MAX_NAME_LEN    (20)
#define N_DAYS           (5)
#define N_HOURS         (14)

/* Error codes */

#define OK              (-1)
#define NO_SUCH_TA      (-2)
#define UNAVAILABLE     (-3)
#define WRITE_ERROR     (-4)
#define BAD_TIME        (-5)

#define BAD_TA ((teaching_assistant_t){\
  .id_number = -1,\
  .first_name = "BOGUS",\
  .last_name = "BOGUS",\
  .gender = -1,\
  .type = -1,\
  .rating = -1.0,\
  .calendar[N_DAYS - 1][N_HOURS - 1] = 'B'})

enum gender_t {
  FEMALE,
  MALE,
  OTHER,
};

enum type_t {
  UTA,
  GTA,
  HEAD,
  COORDINATOR,
};

/* Structure Declarations */

typedef struct {
  int             id_number;
  char            first_name[MAX_NAME_LEN];
  char            last_name[MAX_NAME_LEN];
  enum gender_t   gender;
  enum type_t     type;
  float           rating;
  char            calendar[N_DAYS][N_HOURS];
} teaching_assistant_t;

/* Function prototypes */

teaching_assistant_t read_teaching_assistant(FILE *, int);
int write_teaching_assistant(FILE *, teaching_assistant_t, int);
float compute_percent_by_male(FILE *);
float compute_percent_by_type(FILE *, enum type_t);
float average_rating_by_type(FILE *, enum type_t);
int find_teaching_assistant_by_id(FILE *, int);
int find_teaching_assistant_by_name(FILE *, char *, char *);
int update_teaching_assistant_rating(FILE *, int, float);
int book_office_hours(FILE *, int, int, int);

#endif // HEADER_COMMENTS_73_H
