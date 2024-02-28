/* Minh Tran, hw7.c, CS 24000, Spring 2024
 */

/* Add any includes here */
#include "hw7.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Define any global variables here */

#define IMPOSTER_SECRET "amongus"

/* Define your functions here */

/*
 * This function checks all usernames for errors.
 */

int check_username() {
  for (int i = 0; i < g_player_count; i++) {
    for (int j = 0; j < strlen(g_player_array[i].username); j++) {
      if (!isalpha(g_player_array[i].username[j])) {
        return NOT_FOUND;
      }
    }

    for (int j = 0; j < i; j++) {
      if (strcmp(g_player_array[i].username,
                 g_player_array[j].username) == 0) {
        return BAD_RECORD;
      }
    }
  }
  return 0;
} /* check_username() */

/*
 * This functions reads a list of players from the specified file.
 */

int read_players(char *in_file) {
  assert(in_file != NULL);

  FILE *pointer = 0;
  pointer = fopen(in_file, "r");
  if (pointer == NULL) {
    return NON_READABLE_FILE;
  }

  g_player_count = 0;

  while (1) {
    char username[MAX_NAME_LEN] = {0};
    char buff[MAX_BUFF_LEN] = {0};
    int status = 0;

    status = fscanf(pointer, "%19[^$]$%199[^$]$", username, buff);
    if (status == EOF) {
      break;
    }
    if (status < 2) {
      fclose(pointer);
      pointer = NULL;
      return BAD_RECORD;
    }

    char friends[MAX_FRIENDS][MAX_NAME_LEN] = {0};
    int trust[MAX_FRIENDS] = {0};
    int friend_count = 0;
    int trust_count = 0;
    char spare = '\0';

    while (1) {
      status = fscanf(pointer, "+%19[^+$]", friends[friend_count]);
      if (status < 1) {
        break;
      }
      friend_count++;
      if (friend_count == MAX_FRIENDS) {
        break;
      }
    }

    status = fscanf(pointer, "%c", &spare);
    if ((status < 1) || (spare != '$')) {
      fclose(pointer);
      pointer = NULL;
      return BAD_RECORD;
    }
    while (1) {
      status = fscanf(pointer, "+%d", &trust[trust_count]);
      if (status < 1) {
        break;
      }
      trust_count++;
      if (trust_count == MAX_FRIENDS) {
        break;
      }
    }

    if (friend_count != trust_count) {
      fclose(pointer);
      pointer = NULL;
      return BAD_RECORD;
    }

    status = fscanf(pointer, "%c", &spare);
    if (status < 1) {
      fclose(pointer);
      pointer = NULL;
      return BAD_RECORD;
    }

    if (g_player_count == MAX_PLAYERS) {
      fclose(pointer);
      pointer = NULL;
      return TOO_MUCH_DATA;
    }

    strncpy(g_player_array[g_player_count].username,
            username, MAX_NAME_LEN - 1);
    g_player_array[g_player_count].username[MAX_NAME_LEN - 1] = '\0';

    strncpy(g_player_array[g_player_count].secret,
            buff, KEY_LEN - 1);
    g_player_array[g_player_count].secret[KEY_LEN - 1] = '\0';

    for (int i = 0; i < MAX_FRIENDS; i++) {
      if (i >= friend_count) {
        g_player_array[g_player_count].friends[i][0] = '\0';
        g_player_array[g_player_count].trust[i] = 0;
      }
      else {
        strncpy(g_player_array[g_player_count].friends[i],
                friends[i], MAX_NAME_LEN - 1);
        g_player_array[g_player_count].friends[i][MAX_NAME_LEN - 1] = '\0';
        g_player_array[g_player_count].trust[i] = trust[i];
      }

      g_player_array[g_player_count].friend_ptrs[i] = NULL;
    }

    g_player_count++;
  }

  fclose(pointer);
  pointer = NULL;

  if (g_player_count == 0) {
    return NO_PLAYERS;
  }

  int check = check_username();
  if (check != 0) {
    return check;
  }

  return g_player_count;
} /* read_players() */

/*
 * This functions assigns the pointers of a player to their friends.
 */

void connect_players() {
  for (int i = 0; i < g_player_count; i++) {
    for (int j = 0; j < MAX_FRIENDS; j++) {
      if (g_player_array[i].trust[j] == 0) {
        break;
      }
      for (int k = 0; k < g_player_count; k++) {
        if (strcmp(g_player_array[k].username,
                   g_player_array[i].friends[j]) == 0) {
          g_player_array[i].friend_ptrs[j] = &g_player_array[k];
          break;
        }
      }
    }
  }
} /* connect_players() */

/*
 * This function returns the length of the path of friends
 * with maximum trust.
 */

int length_of_max_trust_friend_path(player_t *player) {
  assert(player != NULL);

  int traverse = 0;

  while (1) {
    if (traverse > g_player_count) {
      return PATH_IS_CYCLE;
    }

    int max_trust = 0;
    int id = 0;
    for (int i = 0; i < MAX_FRIENDS; i++) {
      if (player->trust[i] > max_trust) {
        max_trust = player->trust[i];
        id = i;
      }
    }

    if (max_trust == 0) {
      break;
    }

    traverse++;
    player = player->friend_ptrs[id];
  }

  return traverse;
} /* length_of_max_trust_friend_path() */

/*
 * This function finds imposters in the path of friends
 * with minimum trust.
 */

int imposters_in_min_trust_friend_path(player_t *player,
                                       char (*imposters)[MAX_NAME_LEN]) {
  assert(player != NULL);
  assert(imposters != NULL);

  int traverse = 0;
  int imposter_count = 0;

  while (1) {
    if (traverse > g_player_count) {
      return PATH_IS_CYCLE;
    }

    if (strstr(player->secret, IMPOSTER_SECRET)) {
      strncpy(imposters[imposter_count], player->username,
              MAX_NAME_LEN - 1);
      imposters[imposter_count][MAX_NAME_LEN - 1] = '\0';
      imposter_count++;
    }

    int min_trust = player->trust[0];
    int id = 0;
    for (int i = 1; i < MAX_FRIENDS; i++) {
      if (player->trust[i] == 0) {
        break;
      }

      if (player->trust[i] < min_trust) {
        min_trust = player->trust[i];
        id = i;
      }
    }

    if (min_trust == 0) {
      break;
    }

    traverse++;
    player = player->friend_ptrs[id];
  }

  return imposter_count;
} /* imposters_in_min_trust_friend_path() */
