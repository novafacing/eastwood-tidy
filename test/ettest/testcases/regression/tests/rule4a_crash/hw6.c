/* Alexander Yang, hw6.c, CS 24000, Spring 2024
 */

#include "hw6.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Reads in a list of players following a specific format. Truncate any input
 * that is longer than the specified max length. Return the appropriate error
 * code if neccesary. Otherwise return the number of players read in.
 */

int read_players(char *in_file) {
  g_player_count = 0;

  assert(in_file != NULL);
  FILE *file = fopen(in_file, "r");
  if (file == NULL) {
    return NON_READABLE_FILE;
  }

  int players_read = 0;
  char delim = '\0';
  char buffer[MAX_BUFF_LEN] = "";
  char username[MAX_NAME_LEN] = "";
  char secret[KEY_LEN] = "";
  char friend[MAX_NAME_LEN] = "";
  int trust;

  while (fscanf(file, "%199[^$]", buffer) != EOF) {
    if (g_player_count >= MAX_PLAYERS) {
      fclose(file);
      file = NULL;
      return TOO_MUCH_DATA;
    }

    // Username

    strncpy(username, buffer, MAX_NAME_LEN);
    fscanf(file, "%c", &delim);
    if (delim != '$') {
      fclose(file);
      file = NULL;
      return BAD_RECORD;
    }
    for (int i = 0; i < g_player_count; i++) {
      if (strcmp(username, g_player_array[i].username) == 0) {
        fclose(file);
        file = NULL;
        return BAD_RECORD;
      }
    }

    // Secret

    fscanf(file, "%199[^$]", buffer);
    strncpy(secret, buffer, MAX_NAME_LEN);
    fscanf(file, "%c", &delim);
    if (delim != '$') {
      fclose(file);
      file = NULL;
      return BAD_RECORD;
    }

    // Friends

    int friend_index = 0;
    while (fscanf(file, "+%199[^+^$]", buffer) == 1) {
      strncpy(friend, buffer, MAX_NAME_LEN);
      if (friend_index >= MAX_FRIENDS) {
        fclose(file);
        return BAD_RECORD;
      }
      strncpy(g_player_array[g_player_count].friends[friend_index], friend,
              MAX_NAME_LEN);
      g_player_array[g_player_count].friends[friend_index][MAX_NAME_LEN - 1] =
          '\0';

      friend_index++;
    }
    for (int i = friend_index; i < MAX_FRIENDS; i++) {
      g_player_array[g_player_count].friends[i][0] = '\0';
    }

    fscanf(file, "%c", &delim);

    // Trust

    int trust_index = 0;
    while (fscanf(file, "+%d", &trust) == 1) {
      if (trust_index >= MAX_FRIENDS) {
        fclose(file);
        file = NULL;
        return BAD_RECORD;
      }

      g_player_array[g_player_count].trust[trust_index] = trust;

      trust_index++;
    }

    for (int i = trust_index; i < MAX_FRIENDS; i++) {
      g_player_array[g_player_count].trust[i] = 0;
    }

    if (trust_index != friend_index) {
      fclose(file);
      file = NULL;
      return BAD_RECORD;
    }

    fscanf(file, "%c\n", &delim);

    strncpy(g_player_array[g_player_count].username, username, MAX_NAME_LEN);
    g_player_array[g_player_count].username[MAX_NAME_LEN - 1] = '\0';
    strncpy(g_player_array[g_player_count].secret, secret, KEY_LEN);
    g_player_array[g_player_count].secret[KEY_LEN - 1] = '\0';

    g_player_count++;
    players_read++;
  }

  fclose(file);
  file = NULL;
  if (players_read == 0) {
    return NO_PLAYERS;
  }

  for (int i = 0; i < g_player_count; i++) {
    for (int j = 0; j < MAX_FRIENDS; j++) {
      if (g_player_array[i].friends[j][0] != '\0') {
        int friend_exists = 0;
        for (int k = 0; k < g_player_count; k++) {
          if (strcmp(g_player_array[i].friends[j],
                     g_player_array[k].username) == 0) {
            friend_exists = 1;
            break;
          }
        }
        if (!friend_exists) {
          return NOT_FOUND;
        }
      }
    }
  }

  return players_read;
} /* read_players() */

/* .. */

int find_imposters() { return 0; } /* find_imposters() */