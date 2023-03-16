/* Name, hw3.c, CS 24000, Spring 2023
 * Last updated Dec 8, 2022
 */

/* Add any includes here */
#include "hw3.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Define your global variables here */
int g_player_count = 0;
char g_player_name[MAX_PLAYERS][NUM_ATTR][MAX_NAME_LEN];
int g_attributes[MAX_PLAYERS][NUM_ATTR];
int g_shooting[MAX_PLAYERS][NUM_COLS];
int g_statistics[MAX_PLAYERS][NUM_COLS];

/*
 * It will open the file specified by the first argument and
 * read in the records, filling the
 * g_player_name, g_attributes, g_shootings and g_statistics arrays.
 */

int read_tables(char *in_file) {
  char player_name[MAX_NAME_LEN] = "";
  char position[MAX_NAME_LEN] = "";
  int height = 0;
  char height_unit[MAX_UNIT_LEN] = "";
  int weight = 0;
  char weight_unit[MAX_UNIT_LEN] = "";
  int att_2 = 0;
  int made_2 = 0;
  int att_3 = 0;
  int made_3 = 0;
  int att_ft = 0;
  int made_ft = 0;
  int rebound = 0;
  int assist = 0;
  int steal = 0;
  int block = 0;
  int turnover = 0;
  int foul = 0;
  int number_of_elements = 0;
  int count = 0;
  int index_name = 0;
  int index_position = 0;
  int index_height = 0;
  int index_weight = 0;
  int index_player = 0;
  double double_height = 0.0;
  double double_weight = 0.0;
  char whole_line[200] = "";

  FILE *fp = 0;
  fp = fopen(in_file, "r");

  /* File read error check begins */
  if (fp == NULL) {
    g_player_count = 0;
    return FILE_READ_ERR;
  }
  /* File read error check ends */

  /* no data points check begins */
  if (fscanf(fp, "%s\n", whole_line) == EOF) {
    fclose(fp);
    g_player_count = 0;
    return NO_DATA_POINTS;
  }
  /* no data points check begins */

  /* record error starts */
  fseek(fp, 0, SEEK_SET);
  while (1) {
    number_of_elements = fscanf(fp, "%49[^,],%49[^|]|%d%3[^,],%d%3[^|]|%d,"
                                    "%d,%d,%d,%d,%d|%d,%d,%d,%d,%d,%d\n",
                                player_name, position, &height, height_unit,
                                &weight, weight_unit, &att_2, &made_2, &att_3,
                                &made_3, &att_ft, &made_ft, &rebound, &assist,
                                &steal, &block, &turnover, &foul);
    if (number_of_elements == -1) {
      break;
    } else if (number_of_elements != 18) {
      fclose(fp);
      g_player_count = 0;
      return RECORD_ERROR;
    }
  }
  /* record error ends */

  /* bad data check begins */
  fseek(fp, 0, SEEK_SET);
  while (fscanf(fp,
                "%49[^,],%49[^|]|%d%3[^,],%d%3[^|]|%d,%d,%d,%d,"
                "%d,%d|%d,%d,%d,%d,%d,%d\n",
                player_name, position, &height, height_unit,
                &weight, weight_unit, &att_2, &made_2, &att_3,
                &made_3, &att_ft, &made_ft, &rebound, &assist,
                &steal, &block, &turnover, &foul) != EOF) {
    if (att_2 < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (made_2 < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (att_3 < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (made_3 < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (att_ft < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (made_ft < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (rebound < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (assist < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (steal < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (block < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (turnover < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }

    if (foul < 0) {
      fclose(fp);
      g_player_count = 0;
      return BAD_DATA;
    }
  }
  /* bad data check ends */

  /* out of bounds check starts */
  fseek(fp, 0, SEEK_SET);
  while (fscanf(fp,
                "%49[^,],%49[^|]|%d%3[^,],%d%3[^|]|%d,%d,%d,%d,"
                "%d,%d|%d,%d,%d,%d,%d,%d\n",
                player_name, position, &height, height_unit,
                &weight, weight_unit, &att_2, &made_2, &att_3,
                &made_3, &att_ft, &made_ft, &rebound, &assist,
                &steal, &block, &turnover, &foul) != EOF) {
    count++;
  }
  if (count > MAX_PLAYERS) {
    g_player_count = count;
    fclose(fp);
    return OUT_OF_BOUNDS;
  }
  /* out of bounds check ends */

  /* assign values to g_player_name */
  fseek(fp, 0, SEEK_SET);
  while (fscanf(fp,
                "%49[^,],%49[^|]|%d%3[^,],%d%3[^|]|%d,%d,%d,%d,"
                "%d,%d|%d,%d,%d,%d,%d,%d\n",
                player_name, position, &height, height_unit,
                &weight, weight_unit, &att_2, &made_2, &att_3,
                &made_3, &att_ft, &made_ft, &rebound, &assist,
                &steal, &block, &turnover, &foul) != EOF) {
    index_position = 0;
    strncpy(g_player_name[index_name][index_position], player_name,
            sizeof(g_player_name[index_name][index_position]));
    index_position++;
    strncpy(g_player_name[index_name][index_position], position,
            sizeof(g_player_name[index_name][index_position]));
    index_name++;
    //lec7 page 35
  }

  /* assign values to g_attributes */
  fseek(fp, 0, SEEK_SET);
  while (fscanf(fp,
                "%49[^,],%49[^|]|%d%3[^,],%d%3[^|]|%d,%d,%d,%d,"
                "%d,%d|%d,%d,%d,%d,%d,%d\n",
                player_name, position, &height, height_unit,
                &weight, weight_unit, &att_2, &made_2, &att_3,
                &made_3, &att_ft, &made_ft, &rebound, &assist,
                &steal, &block, &turnover, &foul) != EOF) {
    index_height = 0;
    if (strcmp(height_unit, "cm") == 0) {
      double_height = (double) height * IN_PER_CM;
      height = height * IN_PER_CM;
      if (double_height - (double) height > 0.5) {
        height++;
        g_attributes[index_weight][index_height] = height;
      } else {
        g_attributes[index_weight][index_height] = height;
      }
    } else {
      g_attributes[index_weight][index_height] = height;
    }
    index_height++;

    if (strcmp(weight_unit, "kg") == 0) {
      double_weight = (double) weight * LB_PER_KG;
      weight = weight * LB_PER_KG;
      if (double_weight - (double) weight > 0.5) {
        weight++;
        g_attributes[index_weight][index_height] = weight;
      } else {
        g_attributes[index_weight][index_height] = weight;
      }
    } else {
      g_attributes[index_weight][index_height] = weight;
    }
    index_weight++;
  }


  /* assign values to g_shooting and g_statistics */
  fseek(fp, 0, SEEK_SET);
  while (fscanf(fp,
                "%49[^,],%49[^|]|%d%3[^,],%d%3[^|]|%d,%d,%d,%d,"
                "%d,%d|%d,%d,%d,%d,%d,%d\n",
                player_name, position, &height, height_unit,
                &weight, weight_unit, &att_2, &made_2, &att_3,
                &made_3, &att_ft, &made_ft, &rebound, &assist,
                &steal, &block, &turnover, &foul) != EOF) {
    for (int i = 0; i < 6; i++) {
      if (i == 0) {
        g_shooting[index_player][i] = att_2;
        g_statistics[index_player][i] = rebound;
      } else if (i == 1) {
        g_shooting[index_player][i] = made_2;
        g_statistics[index_player][i] = assist;
      } else if (i == 2) {
        g_shooting[index_player][i] = att_3;
        g_statistics[index_player][i] = steal;
      } else if (i == 3) {
        g_shooting[index_player][i] = made_3;
        g_statistics[index_player][i] = block;
      } else if (i == 4) {
        g_shooting[index_player][i] = att_ft;
        g_statistics[index_player][i] = turnover;
      } else if (i == 5) {
        g_shooting[index_player][i] = made_ft;
        g_statistics[index_player][i] = foul;
      }
    }
    index_player++;
  }
  /* assign features ends */

  g_player_count = count;
  fclose(fp);
  return g_player_count;
} /* read_tables() */

/*
 * It will return the index of player with the most points made
 */

int find_top_scorer(void) {
  /* no data points check begins */
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  /* no data points check begins */

  int index = 0;
  int max_score = 0;
  int next_one_score = 0;
  max_score = g_shooting[0][1] * 2 + g_shooting[0][3] * 3 + g_shooting[0][5];
  /* calculation begins */
  for (int i = 0; i < g_player_count; i++) {
    next_one_score =
            g_shooting[i][1] * 2 + g_shooting[i][3] * 3 + g_shooting[i][5];
    if (max_score < next_one_score) {
      index = i;
      max_score = next_one_score;
    }
  }
  /* calculation ends */
  return index;
} /* find_top_scorer() */

/*
 * It will return the index of the most accurate player, which is found by
 * finding the percentage of attempts that are successful
 */

int find_most_accurate_player(void) {
  /* no data points check begins */
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  /* no data points check begins */

  double accuracy = 0.0;
  double next_accuracy = 0.0;
  int check = 0;
  int index = 0;
  int height = 0;
  int attempt = 0;
  int next_height = 0;
  int next_attempt = 0;

  /* no attempts check begins */
  for (int i = 0; i < g_player_count; i++) {
    for (int j = 0; j < 5; j++) {
      if (g_shooting[i][j] != 0) {
        check = 1;
      }
    }
  }
  if (check == 0) {
    return NO_ATTEMPTS;
  }
  /* no attempts check ends */

  /* calculation begins */
  accuracy = (double) (g_shooting[0][1] + g_shooting[0][3] + g_shooting[0][5])
             /
             (double) (g_shooting[0][0] + g_shooting[0][2] + g_shooting[0][4]);

  for (int i = 1; i < g_player_count; i++) {
    next_accuracy =
            (double) (g_shooting[i][1] + g_shooting[i][3] + g_shooting[i][5])
            / (double) (g_shooting[i][0] + g_shooting[i][2] + g_shooting[i][4]);
    next_height = g_attributes[i][0];
    next_attempt = g_shooting[i][0] + g_shooting[i][2] + g_shooting[i][4];

    if (accuracy < next_accuracy) {
      index = i;
      accuracy = next_accuracy;
    }
    else if (accuracy == next_accuracy) {
      attempt = g_shooting[i][0] + g_shooting[i][2] +
                      g_shooting[i][4];
      height = g_attributes[index][0];

      if (attempt < next_attempt) {
        index = i;
      }
      else if (attempt == next_attempt) {
        if (height > next_height) {
          index = i;
        }
      }
    }
  }
  /* calculation ends */
  return index;
} /* find_most_accurate_player() */

/*
 * It will return the number of players
 * that are within the given weight range
 */

int count_players_in_weight_range(int lower_bound, int upper_bound) {
  /* no data points check begins */
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  /* no data points check begins */

  /* bad range check begins */
  if (lower_bound > upper_bound) {
    return BAD_RANGE;
  }
  /* bad range check begins */

  int number = 0;
  for (int i = 0; i < g_player_count; i++) {
    if ((g_attributes[i][1] >= lower_bound) &&
        (g_attributes[i][1] <= upper_bound)) {
      number++;
    }
  }

  /* no such data check begins */
  if (number == 0) {
    return NO_SUCH_DATA;
  }
  /* no such data check begins */

  return number;
} /* count_players_in_weight_range() */

/*
 * It will return maximum total number of
 * attempts of a player with position specified
 */

int find_most_attempts_player(char *given_position) {
  /* no data points check begins */
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  /* no data points check begins */

  int number = 0;
  int attempt = 0;
  int next_attempt = 0;
  char position[MAX_NAME_LEN] = "";

  position[0] = toupper(given_position[0]);
  for (int i = 1; i < strlen(given_position); i++) {
    position[i] = tolower(given_position[i]);
  }
  for (int i = 1; i < strlen(given_position); i++) {
    if (given_position[i] == 32) {
      position[i + 1] = toupper(given_position[i + 1]);
    }
  }

  for (int i = 0; i < g_player_count; i++) {
    if (strcmp(g_player_name[i][1], position) == 0) {
      number++;
      attempt = g_shooting[i][0] + g_shooting[i][2] + g_shooting[i][4];
      break;
    }
  }

  /* no such data check begins */
  if (number == 0) {
    return NO_SUCH_DATA;
  }
  /* no such data check ends */

  for (int i = number; i < g_player_count; i++) {
    if (strcmp(g_player_name[i][1], position) == 0) {
      next_attempt = g_shooting[i][0] + g_shooting[i][2] + g_shooting[i][4];
    }
    if (attempt < next_attempt) {
      attempt = next_attempt;
    }
  }

  return attempt;
} /* find_most_attempts_player() */

/*
 * It will return the grade for the
 * given player based on Coach Gearlds’s grading system
 */

float calculate_coach_grade(char *player_name) {
  /* no data points check begins */
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  /* no data points check begins */

  int index = 0;
  int check = 0;
  float grade = 0.0f;
  for (int i = 0; i < g_player_count; i++) {
    if (strcmp(g_player_name[i][0], player_name) == 0) {
      check = 1;
      break;
    } else {
      index++;
    }
  }

  /* no such data check begins */
  if (check == 0) {
    return NO_SUCH_DATA;
  }
  /* no such data check ends */

  /* calculation begins */
  grade = ((float) g_statistics[index][0] * 25 +
           (float) g_statistics[index][1] * 20 +
           (float) g_statistics[index][2] * 35 +
           (float) g_statistics[index][3] * 25 -
           (float) g_statistics[index][4] * 15 -
           (float) g_statistics[index][5] * 20) /
          ((float) g_statistics[index][0] + (float) g_statistics[index][1] +
           (float) g_statistics[index][2] + (float) g_statistics[index][3] +
           (float) g_statistics[index][4] + (float) g_statistics[index][5] + 1);
  /* calculation ends */

  /* bad player check begins */
  if (grade < 0) {
    return BAD_PLAYER;
  }
  /* bad player check ends */

  return grade;
} /* calculate_coach_grade() */

/*
 * It will write the grade for each player to the given out_file
 */

int calculate_all_players_grade(char *out_file) {
  float grade = 0.0f;
  FILE *fp = 0;
  fp = fopen(out_file, "w");

  /* File write error check begins */
  if (fp == NULL) {
    return FILE_WRITE_ERR;
  }
  /* File write error check ends */

  /* no data points check begins */
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  /* no data points check begins */

  for (int i = 0; i < g_player_count; i++) {
    grade = calculate_coach_grade(g_player_name[i][0]);
    if (grade > 0) {
      fprintf(fp, "%s: %.2f\n", g_player_name[i][0], grade);
    } else {
      fprintf(fp, "%s: 0.00\n", g_player_name[i][0]);
    }
  }
  fclose(fp);
  return OK;
} /* calculate_all_players_grade() */

/*
 * It will write the players all data into a table
 */

int write_tables(char *out_file, int start_row, int end_row) {
  FILE *fp = 0;
  fp = fopen(out_file, "w");

  /* File write error check begins */
  if (fp == NULL) {
    return FILE_WRITE_ERR;
  }
  /* File write error check ends */

  /* no data points check begins */
  if (g_player_count == 0) {
    fclose(fp);
    return NO_DATA_POINTS;
  }
  /* no data points check begins */

  /* bad row check begins */
  if ((start_row < 0) || (end_row >= g_player_count) || (end_row < start_row)) {
    fclose(fp);
    return BAD_ROW;
  }
  /* bad row check begins */

  /* write table begins */
  for (int i = start_row; i <= end_row; i++) {
    fprintf(fp, "%s,%s|%din,%dlbs|%d,%d,%d,%d,%d,%d|%d,%d,%d,%d,%d,%d\n",
            g_player_name[i][0], g_player_name[i][1], g_attributes[i][0],
            g_attributes[i][1], g_shooting[i][0], g_shooting[i][1],
            g_shooting[i][2], g_shooting[i][3], g_shooting[i][4],
            g_shooting[i][5], g_statistics[i][0], g_statistics[i][1],
            g_statistics[i][2], g_statistics[i][3], g_statistics[i][4],
            g_statistics[i][5]);
  }
  /* write table ends */
  fclose(fp);
  return OK;
} /* write_tables() */

/* Remember, you don't need a main function!
 * It is provided by hw3_main.c or hw3_test.o
 */

/*
      printf("player name: %s\n"
           "position: %s\n"
           "height: %d%s\n"
           "weight: %d%s\n"
           "att_2: %d\n"
           "made_2: %d\n"
           "att_3: %d\n"
           "made_3: %d\n"
           "att_ft: %d\n"
           "made_ft: %d\n"
           "rebound: %d\n"
           "assist: %d\n"
           "steal: %d\n"
           "block: %d\n"
           "turnover: %d\n"
           "foul: %d\n", player_name, position, height, height_unit,
           weight, weight_unit, att_2, made_2, att_3,
           made_3, att_ft, made_ft, rebound, assist,
           steal, block, turnover, foul);
 */
