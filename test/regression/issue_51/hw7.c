/* Name, hw7.c, CS 24000, Fall 2021
 * Last updated Oct 9, 2021
 */

/* Add any includes here */
#include "hw7.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Define any global variables here */

int g_song_count = 0;
song_t g_song_array[MAX_SONGS] = {};

/*
 * read_songs
 */

int read_songs(char *file_name) {
  assert(file_name != NULL);

  FILE *fp = NULL;
  fp = fopen(file_name, "r");
  if (fp == NULL) {
    return NON_READABLE_FILE;
  }

  int count = 0;
  do {
    char song_name[MAX_NAME_LEN] = {'\0'};
    char singer_name[MAX_NAME_LEN] = {'\0'};
    char genre_name[MAX_NAME_LEN] = {'\0'};
    char cover_artist[MAX_NAME_LEN] = {'\0'};

    count = fscanf(fp, "%49[^|]|%49[^|]|%49[^|]|%49[^;];\n", song_name,
                   singer_name, genre_name, cover_artist);
    if ((count == 0) || (count == -1)) {
      fclose(fp);
      fp = NULL;
      if (g_song_count == 0) {
        return NO_SONGS;
      }
      return g_song_count;
    }

    if (count != 4) {
      fclose(fp);
      fp = NULL;
      return BAD_RECORD;
    }
    g_song_count++;
    if (g_song_count > MAX_SONGS) {
      fclose(fp);
      fp = NULL;
      return OUT_OF_BOUND;
    }

    strncpy(g_song_array[g_song_count - 1].song_name, song_name, MAX_NAME_LEN - 1);
    strncpy(g_song_array[g_song_count - 1].singer_name, singer_name, MAX_NAME_LEN - 1);
    strncpy(g_song_array[g_song_count - 1].genre_name, genre_name, MAX_NAME_LEN - 1);
    strncpy(g_song_array[g_song_count - 1].cover_artist, cover_artist, MAX_NAME_LEN - 1);
    g_song_array[g_song_count - 1].next_cover_ptr = NULL;
    g_song_array[g_song_count - 1].next_song_ptr = NULL;

  } while (count != EOF);

  fclose(fp);
  fp = NULL;
  return g_song_count;

} /* read_songs() */

/*
 * link_songs()
 */

void link_songs() {

  for (int i = 0; i < g_song_count; i++) {
    for  (int j = i; j < g_song_count; j++) {
      int cover_artist_length = 0;
      cover_artist_length = strlen(g_song_array[i].cover_artist);
      int singer_name_length = 0;
      singer_name_length = strlen(g_song_array[i].singer_name);


      if ((strncmp(g_song_array[i].cover_artist, g_song_array[j].cover_artist, cover_artist_length) == 0)
          && (strncmp(g_song_array[i].singer_name, g_song_array[j].singer_name, singer_name_length) != 0)
          && (i != j) && (g_song_array[i].next_cover_ptr == NULL)) {
        g_song_array[i].next_cover_ptr = &g_song_array[j];
      }
      int genre_name_length = 0;
      genre_name_length = strlen(g_song_array[i].genre_name);

      if ((strncmp(g_song_array[i].genre_name, g_song_array[j].genre_name,
                   genre_name_length) == 0) &&
          (i != j) && (g_song_array[i].next_song_ptr == NULL)) {
        g_song_array[i].next_song_ptr = &g_song_array[j];
      }

    }
  }

} /* link_songs() */

/*
 * find_song_with_singer_name
 */

int find_song_with_singer_name(int song_index, char *singer_name) {
  assert((song_index >= 0) && (song_index < g_song_count));
  assert(singer_name != NULL);

  song_t *initial_ptr = g_song_array[song_index].next_song_ptr;
  while (initial_ptr != NULL) {
    song_t song = *initial_ptr;
    int singer_name_length = 0;
    singer_name_length = strlen(song.singer_name);
    if (strncmp(song.singer_name, singer_name, singer_name_length) == 0) {
      return SONG_FOUND;
    }
    initial_ptr = song.next_song_ptr;
  }

  return SONG_NOT_FOUND;

} /* find_song_with_singer_name() */

/*
 * count_song_cover_links
 */

int count_song_cover_links(int song_index) {
  assert((song_index >= 0) && (song_index < g_song_count));

  song_t *initial_ptr = NULL;
  initial_ptr = g_song_array[song_index].next_cover_ptr;
  int songs_counted = 0;
  while (initial_ptr != NULL) {
    songs_counted++;
    song_t song = {};
    song = *initial_ptr;
    initial_ptr = song.next_cover_ptr;
  }

  return songs_counted;

} /* count_song_cover_links() */

/*
 * find_songs_by_genre_name
 */

int find_songs_by_genre_name(char *genre_name, int *songs_array) {
  assert(genre_name != NULL);

  int song_size = sizeof &songs_array / sizeof songs_array[0];
  for (int i = 0; i < song_size; i++) {
    assert(&songs_array[i] != NULL);
  }

  int songs_found =  0;
  for (int i = 0; i < g_song_count; i++) {
    int genre_name_length = 0;
    if (strlen(g_song_array[i].genre_name) < MAX_NAME_LEN) {
      genre_name_length = strlen(g_song_array[i].genre_name);
    } else {
      genre_name_length = MAX_NAME_LEN;
    }
    if (strncmp(g_song_array[i].genre_name, genre_name, genre_name_length) == 0) {
      songs_array[songs_found] = i;
      songs_found++;
    }
  }


  return songs_found;
} /* find_song_by_genre_name() */

/* Define your functions here */

