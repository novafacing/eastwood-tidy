#ifndef HW5_H
#define HW5_H

#include <stdio.h>

/* Constant definitions */
#define MAX_NAME_LEN    (20)  /* Max length of any string */
#define BAD_ID          (-10)
#define N_DAYS          (5)
#define N_HOURS         (9)

/* Error codes */
#define OK              (-1)   /* No errors, everything as should be */
#define NO_DOCTOR       (-2)
#define UNAVAILABLE     (-3)
#define WRITE_ERR       (-4)
#define BAD_APPOINTMENT (-5)



enum gender_t {
  FEMALE,
  MALE,
  OTHER
};

enum doctor_type_t {
  CARDIOLOGIST,
  AUDIOLOGIST,
  DENTIST,
  RADIOLOGIST
};

#define BAD_DOCTOR ((doctor_t){\
  .id_number = BAD_ID,\
  .first_name = "BOGUS",\
  .last_name = "BOGUS",\
  .gender = -1,\
  .doctor_type = -1,\
  .consultation_fee = -1.0,\
  .calendar[N_DAYS - 1][N_HOURS - 1] = 'A'})



/* Structure Declarations */
typedef struct {
  int id_number;
  char first_name[MAX_NAME_LEN];
  char last_name[MAX_NAME_LEN];
  enum gender_t gender;
  enum doctor_type_t doctor_type;
  float consultation_fee;
  char calendar[N_DAYS][N_HOURS];
} doctor_t;

/* Function prototypes */
doctor_t read_doctor(FILE *, int);
int write_doctor(FILE *, doctor_t, int);
float percent_doctor_by_gender(FILE*, enum gender_t);
float average_consultation_fee_by_type(FILE*, enum doctor_type_t);
doctor_t find_doctor_by_id(FILE*, int);
doctor_t find_doctor_by_name(FILE*, char*, char*, enum doctor_type_t);
int update_doctor_consultation_fee(FILE*, int, float);
float calculate_total_consultation_fee(FILE*, int);
int make_appointment(FILE*, int, int, int);

#endif // HW5_H
