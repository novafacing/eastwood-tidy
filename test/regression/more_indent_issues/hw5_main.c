#include <string.h>
#include <stdio.h>

#include "hw5.h"

void print_doctor(doctor_t doc);
doctor_t read_doctor_from_file(char *);
void print_return_value(int, char *);

/* This function is used to run the different functions implemented in file
 * hw5.c. User input is used to determine which function to run and what
 * input is passed to it. Upon completion, the function returns zero.
 */

int main() {
  char database_file[100] = "";
  FILE * database_fp = 0;
  char doc_file[100] = "";
  char first_name[MAX_NAME_LEN] = "";
  char last_name[MAX_NAME_LEN] = "";
  int return_code = 0;
  float consultation_fee = 0;
  int doc_id = 0;
  int doctor_type = 0;
  int gender = 0;
  int day_val = 0;
  int hour_val = 0;

  doctor_t doc = BAD_DOCTOR;

  printf("\nEnter the name of an input file to use: ");
  fgets(database_file, sizeof(database_file), stdin);
  *(strchr(database_file, '\n')) = '\0';

  database_fp = fopen(database_file, "rb+");
  if (database_fp == NULL) {
    printf("Unable to open %s for input.\n", database_file);
    return 1;
  }

  while (1) {

    printf("\nOPTIONS:\n"
           "0) Quit\n"
           "1) read_employee()\n"
           "2) write_employee()\n"
           "3) percent_doctor_by_gender()\n"
           "4) average_consultation_fee_by_type()\n"
           "5) find_doctor_by_id()\n"
           "6) find_doctor_by_name()\n"
           "7) update_doctor_consultation_fee()\n"
           "8) calculate_total_consultation_fee()\n"
           "9) make_appointment()\n"
           "Select a function: ");
    int choice = 0;
    int status = fscanf(stdin, "%d", &choice);
    /* To prevent infinite loop incase of non integer input. */
    while (getchar() != '\n'){

    }
    if (status <= 0) {
      printf("\nInvalid input! Try again...\n");
      continue;
    }
    if (choice == 0) {
      printf("\nGoodbye!\n\n");
      break;
    }

    int record_num = 0;
    switch (choice) {
      case 0:
        printf("\nGoodbye!\n\n");
        break;
      case 1:
        printf("\nEnter doctor record number: ");
        fscanf(stdin, "%d", &record_num);
        doc = read_doctor(database_fp, record_num);
        print_doctor(doc);
        break;
      case 2:
        printf("\nEnter doctor record number: ");
        fscanf(stdin, "%d", &record_num);
        printf("Enter a file containing an doctor representation: ");
        fscanf(stdin, "\n%s", doc_file);
        doc = read_doctor_from_file(doc_file);
        printf("Read the following information:\n");
        print_doctor(doc);
        return_code = write_doctor(database_fp, doc, record_num);
        print_return_value(return_code, "write_employee");
        break;
      case 3:
        printf("\nEnter doctor's gender (as an integer): ");
        fscanf(stdin, "%d", &gender);
        float percent = percent_doctor_by_gender(database_fp, gender);
        printf("Doctor's gender is  %f percent.", percent);
        break;
      case 4:
        printf("\nEnter doctor's type (as an integer): ");
        fscanf(stdin, "%d", &doctor_type);
        float avg = average_consultation_fee_by_type(database_fp,
                            doctor_type);
        printf("The function returned: %f\n", avg);
        break;
      case 5:
        printf("\nEnter an doctor ID: ");
        fscanf(stdin, "%d", &doc_id);
        doc = find_doctor_by_id(database_fp, doc_id);
        print_doctor(doc);
        break;
      case 6:
        printf("\nEnter a first name: ");
        fscanf(stdin, "\n%39[^\n]", first_name);
        printf("Enter a last name: ");
        fscanf(stdin, "\n%39[^\n]", last_name);
        printf("\nEnter doctor's type (as an integer): ");
        fscanf(stdin, "%d", &doctor_type);
        doc = find_doctor_by_name(database_fp, first_name,
                            last_name, doctor_type);
        print_doctor(doc);
        break;
      case 7:
        printf("\nEnter an doctor ID: ");
        fscanf(stdin, "%d", &doc_id);
        printf("Enter consultation fee: ");
        fscanf(stdin, "%f", &consultation_fee);
        return_code = update_doctor_consultation_fee(database_fp,
                            doc_id, consultation_fee);
        print_return_value(return_code, "update_doctor_consultation_fee");
        break;
      case 8:
        printf("\nEnter an doctor ID: ");
        fscanf(stdin, "%d", &doc_id);
        float total_fee = calculate_total_consultation_fee(database_fp,
                            doc_id);
        printf("the function returned: %f\n", total_fee);
        break;
      case 9:
        printf("\nEnter an doctor ID: ");
        fscanf(stdin, "%d", &doc_id);
        printf("Enter the value for the day:\n");
        fscanf(stdin, "%d", &day_val);
        printf("Enter the value for the hour:\n");
        fscanf(stdin, "%d", &hour_val);
        return_code = make_appointment(database_fp, doc_id, day_val,
                            hour_val);
        printf("\nmake_appointment returned %d.\n", return_code);
        break;
      default:
        printf("\nInvalid selection.\n");
        break;
    }
  }

  return 0;
} /* main() */


/*
 * Function to read in information about an doctor
 */

doctor_t read_doctor_from_file(char *file_name) {
  FILE *fp = fopen(file_name, "r");
  doctor_t doc = BAD_DOCTOR;
  if (fp == NULL){
    return doc;
  }
  if (fscanf(fp, "%d\n", &doc.id_number) != 1) {
    return doc;
  }
  if (fscanf(fp, "%39[^\n]\n", doc.first_name) != 1) {
    return doc;
  }
  if (fscanf(fp, "%39[^\n]\n", doc.last_name) != 1) {
    return doc;
  }
  if (fscanf(fp, "%d\n", ((int *)&doc.gender)) != 1) {
    return doc;
  }
  if (fscanf(fp, "%d\n", ((int *)&doc.doctor_type)) != 1) {
    return doc;
  }
  if (fscanf(fp, "%f\n", &doc.consultation_fee) != 1) {
    return doc;
  }
  for (int i = 0; i < N_DAYS; i++) {
    for (int j = 0; j < N_HOURS; j++) {
      if (fscanf(fp, "%c", &doc.calendar[i][j]) != 1) {
        return doc;
      }
    }
    if (fscanf(fp, "\n") != 0) {
      return doc;
    }
  }
  return doc;
} /* read_doctor_from_file() */


/*
 * Function to print doctor information
 */

void print_doctor(doctor_t doc)
{
  printf("\nID Number: %011d\n", doc.id_number);
  printf("First Name: %s\n", doc.first_name);
  printf("Last Name: %s\n", doc.last_name);
  printf("Gender: ");
  switch (doc.gender) {
    case FEMALE:
      printf("Female\n");
      break;
    case MALE:
      printf("Male\n");
      break;
    case OTHER:
      printf("Other\n");
      break;
    default:
      printf("-1\n");
      break;
  }
  printf("Doctor Type: ");
  switch (doc.doctor_type) {
    case CARDIOLOGIST:
      printf("Cardiologist\n");
      break;
    case AUDIOLOGIST:
      printf("Audiologist\n");
      break;
    case DENTIST:
      printf("Dentist\n");
      break;
    case RADIOLOGIST :
      printf("Radiologist\n");
      break;
    default:
      printf("-1\n");
      break;
  }
  printf("Consultation Fee: $%.2f\n", doc.consultation_fee);
  printf("Calendar:");
  for (int day = 0; day < N_DAYS; day++) {
    printf(" ");
    for (int hour = 0; hour < N_HOURS; hour++) {
      printf("%c", doc.calendar[day][hour]);
    }
  }
  printf("\n");
} /* print_doctor() */


/*
 * Function to print appropriate error codes.
 */

void print_return_value(int val, char *function) {
  printf("Value returned by %s(): %d ", function, val);
  switch (val) {
    case OK:
      printf("(OK)\n");
      break;
    case NO_DOCTOR:
      printf("(NO_DOCTOR)\n");
      break;
    case UNAVAILABLE:
      printf("(UNAVAILABLE)\n");
      break;
    case WRITE_ERR:
      printf("(WRITE_ERR)\n");
      break;
    case BAD_APPOINTMENT:
      printf("(BAD_APPOINTMENT)\n");
      break;
    default:
      printf("\n");
      break;
  }
} /* print_return_value() */
