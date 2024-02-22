/* Name, hw5.c, CS 24000, Spring 2020
 * Last updated February 3, 2020
 */

/* Add any includes here */

#include "hw5.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

/*
 * This function checks the package by making sure the numerical values are
 * greater than 0 and that the package is given a valid priority.
 * Returns true or false.
 */

bool check_package(package_t package) {
  return false;
} /* check_package() */

/*
 * Define read_employee here
 */

package_t read_package(FILE * fp, int offset) {
  assert(fp &&
         (offset >= 0));

  int return_value = fseek(fp, offset * sizeof(package_t), SEEK_SET);
  if (return_value) {
    return BAD_PACKAGE;
  }
  package_t result = BAD_PACKAGE;
  return_value = fread(&result, sizeof(package_t), 1, fp);
  if (return_value != 1) {
    return BAD_PACKAGE;
  }
  return result;
} /* read_package() */


/*
 * Define write_employee here
 */

int write_package(FILE * fp, package_t p, int offset) {
  assert(fp);
  assert(offset >= 0);

  if ((p.id < 0) || (p.order_id < 0) || (p.value < 0.0) || (p.weight < 0.0)) {
    return MALFORMED_PACKAGE;
  }
  fseek(fp, 0, SEEK_END);
  int max_packages = ftell(fp) / sizeof(package_t);

  if (offset >= max_packages) {
    return WRITE_ERR;
  }

  int return_value = fseek(fp, offset * sizeof(package_t), SEEK_SET);
  if (return_value) {
    return WRITE_ERR;
  }

  return_value = fwrite(&p, sizeof(package_t), 1, fp);
  if (return_value != 1) {
    return WRITE_ERR;
  }
  return OK;
} /* write_package() */

/*
 * recv_package
 */

int recv_package(FILE *fp, package_t p) {
  assert(fp);
  if ((p.id < 0) || (p.order_id < 0) || (p.value < 0.0) || (p.weight < 0.0)) {
    return MALFORMED_PACKAGE;
  }

  fseek(fp, 0, SEEK_END);
  int num_packages = ftell(fp) / sizeof(package_t);
  for (int i = 0; i < num_packages; i++) {
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    package_t read_package = BAD_PACKAGE;
    fread(&read_package, sizeof(package_t), 1, fp);
    if (read_package.id != FREE_ID) {
      continue;
    }
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    int return_value = fwrite(&p, sizeof(package_t), 1, fp);
    if (return_value != 1) {
      return WRITE_ERR;
    }
    return i;
  }
  return NO_SPACE;
} /* recv_package() */

/*
 * send_package
 */

int send_package(FILE *fp, int id) {
  assert(fp);
  assert(id > 0);

  fseek(fp, 0, SEEK_END);
  int num_packages = ftell(fp) / sizeof(package_t);
  for (int i = 0; i < num_packages; i++) {
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    package_t read_package = BAD_PACKAGE;
    fread(&read_package, sizeof(package_t), 1, fp);
    if (read_package.id != id) {
      continue;
    }
    read_package.id = FREE_ID;
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    int return_value = fwrite(&read_package, sizeof(package_t), 1, fp);
    if (return_value != 1) {
      return WRITE_ERR;
    }
    return i;
  }
  return NO_PACKAGE;
} /* send_package() */

/*
 * match_route
 */

int match_route(char *route, package_t p) {
  for (int i = 0; i < N_ROUTES; i++) {
    int no_match = 0;
    int last_index = 0;
    for (int j = 0; j < MAX_ROUTE_LEN; j++) {
      char c1 = p.routes[i][j];
      int match = 0;
      if (c1 == '.') {
        break;
      }
      for (int k = last_index; k < MAX_ROUTE_LEN; k++) {
        char c2 = route[k];
        if (c2 == '.') {
          break;
        }
        if (c2 == c1) {
          match = 1;
          last_index = k;
          break;
        }
      }
      if (!match) {
        no_match = 1;
        break;
      }
    }
    if (!no_match) {
      return 1;
    }
  }
  return 0;
} /* match_route() */

/*
 * send_to_route
 */

float send_to_route(FILE * fp, char  * truck_route) {
  assert(fp);
  assert(truck_route);
  printf("%s\n", truck_route);
  double total_weight = 0.0;
  fseek(fp, 0, SEEK_END);
  int num_packages = ftell(fp) / sizeof(package_t);
  for (int i = 0; i < num_packages; i++) {
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    package_t read_package = BAD_PACKAGE;
    fread(&read_package, sizeof(package_t), 1, fp);
    if (!match_route(truck_route, read_package) || read_package.id == FREE_ID) {
      continue;
    }
    read_package.id = FREE_ID;
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    int return_value = fwrite(&read_package, sizeof(package_t), 1, fp);
    if (return_value != 1) {
      return WRITE_ERR;
    }
    printf("%f\n", read_package.weight);
    total_weight += read_package.weight;
  }
  if (total_weight == 0.0) {
    printf("no_package\n");
    return NO_PACKAGE;
  }
  printf("%f\n", total_weight);
  return total_weight;
} /* send_to_route() */

/*
 * find_package_by_id
 */

package_t find_package_by_id(FILE * fp, int id) {
  assert(fp);
  assert(id > 0);

  fseek(fp, 0, SEEK_END);
  int num_packages = ftell(fp) / sizeof(package_t);
  for (int i = 0; i < num_packages; i++) {
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    package_t read_package = BAD_PACKAGE;
    fread(&read_package, sizeof(package_t), 1, fp);
    if (read_package.id != id) {
      continue;
    }
    return read_package;
  }
  return BAD_PACKAGE;
} /* find_package_by_id() */

/*
 * find_shipping_cost
 */

float find_shipping_cost(FILE * fp, int order_id) {
  assert(fp);
  assert(order_id > 0);

  float total_cost = 0.0;
  fseek(fp, 0, SEEK_END);
  int num_packages = ftell(fp) / sizeof(package_t);
  for (int i = 0; i < num_packages; i++) {
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    package_t read_package = BAD_PACKAGE;
    fread(&read_package, sizeof(package_t), 1, fp);
    if (read_package.order_id != order_id || read_package.id == FREE_ID) {
      continue;
    }
    int longest_length = 0;
    for (int i = 0; i < N_ROUTES; i++) {
      int length = 0;
      for(; read_package.routes[i][length] != '.'; length++) {
        // :)
      }
      if (length > longest_length) {
        longest_length = length;
      }
    }
    total_cost += 2.5 * read_package.weight * longest_length;
    if (read_package.weight > 100.0) {
      total_cost += 80.0;
    }
    if (read_package.value > 200.0) {
      total_cost += 50.0;
    }
    if ((read_package.priority == URGENT) || (read_package.priority == PRIME_URGENT)) {
      total_cost += 120.0;
    }
  }
  if (total_cost == 0.0) {
    return NO_PACKAGE;
  }
  return total_cost;
} /* find_shipping_cost() */

/*
 * raise_priority
 */

float raise_priority(FILE * fp, int id) {
  assert(fp);
  assert(id > 0);

  fseek(fp, 0, SEEK_END);
  int num_packages = ftell(fp) / sizeof(package_t);
  for (int i = 0; i < num_packages; i++) {
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    package_t read_package = BAD_PACKAGE;
    fread(&read_package, sizeof(package_t), 1, fp);
    if (read_package.id != id) {
      continue;
    }
    if (read_package.priority == REGULAR) {
      read_package.priority = URGENT;
    }
    if (read_package.priority == PRIME) {
      read_package.priority = PRIME_URGENT;
    }
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    int return_value = fwrite(&read_package, sizeof(package_t), 1, fp);
    if (return_value != 1) {
      return WRITE_ERR;
    }
    return read_package.value;
  }
  return NO_PACKAGE;
} /* raise_package() */

/*
 * combine_package
 */

package_t combine_package(FILE * fp, int oid, char * dest) {
  assert(fp);
  assert(oid > 0);
  assert(dest);

  printf("%s\n", dest);
  package_t combined_package = BAD_PACKAGE;
  fseek(fp, 0, SEEK_END);
  int num_packages = ftell(fp) / sizeof(package_t);
  for (int i = 0; i < num_packages; i++) {
    fseek(fp, i * sizeof(package_t), SEEK_SET);
    package_t read_package = BAD_PACKAGE;
    fread(&read_package, sizeof(package_t), 1, fp);
    if (read_package.order_id != oid || read_package.id == FREE_ID || strcmp(dest, read_package.dest)) {
      continue;
    }
    if (combined_package.id == BAD_ID) {
      combined_package = read_package;
    }
    else {
      combined_package.value += read_package.value;
      combined_package.weight += read_package.weight +0.15 * fmax(combined_package.weight, read_package.weight);
      if (read_package.priority > combined_package.priority) {
        combined_package.priority = read_package.priority;
      }
      for (int i = 0; i < N_ROUTES; i++) {
        for (int j = 0; j < MAX_ROUTE_LEN; j++) {
          char c1 = combined_package.routes[i][j];
          char c2 = read_package.routes[i][j];
          if ((c1 == '.') && (c2 == '-')) {
            memcpy(combined_package.routes[i], read_package.routes[i], MAX_ROUTE_LEN);
            break;
          }
        }
      }
    }
  }
  return combined_package;
} /* combine_package() */