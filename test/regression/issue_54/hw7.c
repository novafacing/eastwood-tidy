/* Name, hw7.c, CS 24000, Fall 2021
 * Last updated Oct 9, 2021
 */

/* Add any includes here */
#include "hw7.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* Define any global variables here */

int g_song_count = 0;
song_t g_song_array[MAX_SONGS] = {};

/* Define your functions here */

/*
 * This function reads in a list of formatted songs from a given file and
 * stores it into a global struct called g_song_array.
 */

int read_songs(char *file) {
  while (1) {
    // This shouldn't flag
    g_song_array[g_song_count - 1].next_cover_ptr = NULL;
    // Neither should this
    g_song_array[g_song_count - 1].next_song_ptr = NULL;

    // This should
    int b = 1;
    int a = (b = 10) == 10;
  }

  return 1;
} /* read_songs() */