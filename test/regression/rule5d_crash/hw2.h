#ifndef HW2_H
#define HW2_H

/* Constant Definitions */

#define MAX_FILE_LEN (50) /* The maximum file name length */
#define MAX_CITY_LEN (50) /* The maximum city name length */

/* Error Codes */

#define OK               (0)
#define FILE_READ_ERR   (-1)
#define FILE_WRITE_ERR  (-2)
#define BAD_RECORD      (-3)
#define NO_DATA_POINTS  (-4)
#define BAD_DATE        (-5)

/* Function Prototypes */

int num_travel_by_month(char *, int, int);
int num_travel_by_destination(char *, char *);
float monthly_report(char *, char *, int, int);
int min_max_price(char *, char *, int);
int compare_files(char *, char *, char *, int);

#pragma GCC poison access

#endif // HW2_H