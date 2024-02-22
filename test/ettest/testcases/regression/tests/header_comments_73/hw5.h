#ifndef HW5_H
#define HW5_H

#include <stdio.h>

/* Constant definitions */
#define MAX_NAME_LEN    (20)  /* Max length of any string */
#define BAD_ID     (-6)
#define FREE_ID (-7)
#define N_ROUTES        (15)
#define MAX_ROUTE_LEN   (10)

/* Error codes */
#define OK                 (-1)   /* No errors, everything as should be */
#define WRITE_ERR          (-4)
#define NO_SPACE           (-5)
#define NO_PACKAGE         (-3)
#define MALFORMED_PACKAGE  (-9) 

#define BAD_PACKAGE ((package_t){\
.id = BAD_ID, \
.order_id = BAD_ID, \
.dest = "FREE", \
.routes[N_ROUTES -1][MAX_ROUTE_LEN -1] = 'X' \
})




enum priority_t{
  REGULAR,
  URGENT,
  PRIME,
  PRIME_URGENT};

/* Structure Declarations */
typedef struct {
  int id;
  int order_id;
  char dest[MAX_NAME_LEN];
  enum priority_t priority;
  float value;
  float weight;
  char routes[N_ROUTES][MAX_ROUTE_LEN];
} package_t;

/* Function prototypes */
package_t read_package(FILE *, int);
int write_package(FILE *, package_t, int);
int recv_package(FILE *, package_t);
int send_package(FILE *, int);
float send_to_route(FILE *, char * );
package_t find_package_by_id(FILE *, int);
float find_shipping_cost(FILE *, int);
float raise_priority(FILE *, int);
package_t combine_package(FILE *, int, char * );

#endif // HW5_H