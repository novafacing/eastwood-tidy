/* Allie Coia, hw8.c, CS 24000, Fall 2021
 * Last updated Oct 17, 2021
 */

/* Add any includes here */
#include "hw8.h"

#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define any global variables here */


/* Define your functions here */

/*
 * Define add_song_to_head here:
 * this function ppends song with the given title and artist to the top of a
 * song list and returns a pointer to the new head
 */

song_t *add_song_to_head(song_t *head, char *title, char *artist) {
  assert(title != NULL);
  assert(artist != NULL);

  song_t *song_ptr = malloc(sizeof(song_t));
  assert(song_ptr != NULL);
  song_ptr->title = malloc(strlen(title) + 1);
  assert(title != NULL);
  song_ptr->artist = malloc(strlen(artist) + 1);
  assert(artist != NULL);

  song_ptr->next_song = head;
  strcpy(song_ptr->title, title);
  strcpy(song_ptr->artist, artist);
  return song_ptr;

} /* add_song_to_head() */

/*
 * Define remove_song_from_head here:
 * A function that removes the head of a list of songs and returns a pointer to
 * the new head or NULL if the list was or became empty
 */

song_t *remove_song_from_head(song_t *ptr) {
  if (ptr == NULL) {
    return NULL;
  }
  song_t *new_head = ptr->next_song;
  free(ptr->title);
  ptr->title = NULL;
  free(ptr->artist);
  ptr->artist = NULL;
  free(ptr);
  ptr = NULL;
  if (new_head == NULL) {
    return NULL;
  }
  return new_head;

} /* remove_song_from_head() */

/*
 * Define add_song_to_tail here:
 * A function that adds a song with the given title and artist to the tail of
 * a list of songs and returns a pointer to the head of that list
 */

song_t *add_song_to_tail(song_t *ptr, char *title, char *artist) {
  assert(title != NULL);
  assert(artist != NULL);

  song_t *song_ptr = malloc(sizeof(song_t));
  assert(song_ptr != NULL);
  song_ptr->title = malloc(strlen(title) + 1);
  assert(title != NULL);
  song_ptr->artist = malloc(strlen(artist) + 1);
  assert(artist != NULL);

  song_t *head = ptr;
  song_ptr->next_song = NULL;

  if (ptr == NULL) {
    strcpy(song_ptr->title, title);
    strcpy(song_ptr->artist, artist);
    return song_ptr;
  }
  while (1) {
    if (ptr->next_song == NULL) {
      strcpy(song_ptr->title, title);
      strcpy(song_ptr->artist, artist);
      ptr->next_song = song_ptr;
      return head;
    }
    ptr = ptr->next_song;
  }
  return head;
} /* add_song_to_tail() */

/*
 * Define remove_song_from_tail here:
 * A function that removes the tail of the given list and returns the head of
 * that list or NULL if it was or became empty
 */

song_t *remove_song_from_tail(song_t *ptr) {
  if (ptr == NULL) {
    return NULL;
  }

  song_t *head = ptr;
  if (head->next_song == NULL) {
    free(ptr->title);
    ptr->title = NULL;
    free(ptr->artist);
    ptr->artist = NULL;
    free(ptr);
    ptr = NULL;
    return NULL;
  }

  song_t *save_ptr = NULL;
  while (1) {
    if (ptr->next_song == NULL) {
      save_ptr = ptr;
      break;
    }
    ptr = ptr->next_song;
  }

  ptr = head;
  while (1) {
    if (ptr->next_song == save_ptr) {
      ptr->next_song = NULL;
      free(save_ptr->title);
      save_ptr->title = NULL;
      free(save_ptr->artist);
      save_ptr->artist = NULL;
      free(save_ptr);
      save_ptr = NULL;
      return head;
    }
    ptr = ptr->next_song;
  }
  return head;
} /* remove_song_from_tail() */

/*
 * Define count_songs here:
 * A function that returns the number of songs in a given list
 */

int count_songs(song_t *ptr) {
  if (ptr == NULL) {
    return 0;
  }

  int song_count = 0;

  while (ptr != NULL) {
    song_count++;
    ptr = ptr->next_song;
  }
  return song_count;

} /* count_songs() */

/*
 * Define search_by_index here:
 * A function that searches a given list for a given index and returns the
 * corresponding song_t, or NULL if the index is larger than the list size
 */

song_t *search_by_index(song_t *ptr, int index) {
  assert(index > 0);

  if (index > count_songs(ptr)) {
    return NULL;
  }

  for (int i = 1; i < index; i++) {
    ptr = ptr->next_song;
  }
  return ptr;
} /* search_by_index() */

/*
 * Define search_by_title_and_artist here:
 * A function that searches the given list with given title and artist and
 * returns the song if found, or NULL if not
 */

song_t *search_by_title_and_artist(song_t *ptr, char *title, char *artist) {
  assert(title != NULL);
  assert(artist != NULL);

  while (ptr != NULL) {
    if ((strcmp(ptr->title, title) == 0) &&
       (strcmp(ptr->artist, artist) == 0)) {
      return ptr;
    }
    ptr = ptr->next_song;
  }
  return NULL;
} /* search_by_title_and_artist() */

/*
 * Define modify_song_by_index here:
 * A function that searches for a song in a given list witha given index and
 * modifies it with the new title and artist returning the new song
 */

song_t *modify_song_by_index(song_t *ptr, int index, char *title,
                             char *artist) {
  assert(index > 0);
  assert(title != NULL);
  assert(artist != NULL);

  if (index > count_songs(ptr)) {
    return NULL;
  }

  for (int i = 1; i < index; i++) {
    ptr = ptr->next_song;
  }
  free(ptr->title);
  ptr->title = malloc(strlen(title) + 1);
  assert(title != NULL);
  ptr->title = title;
  free(ptr->artist);
  ptr->artist = malloc(strlen(artist) + 1);
  assert(artist != NULL);
  ptr->artist = artist;
  return ptr;
} /* modify_song_by_index() */

/*
 * Define move_to_tail here:
 * A function that moves the given amount of songs in a list from the head to
 * the tail and returns a pointer to the head of the list
 */

song_t *move_to_tail(song_t *ptr, int num_songs_moved) {
  assert(num_songs_moved >= 0);
  assert(num_songs_moved <= count_songs(ptr));

  for (int i = 0; i < num_songs_moved; i++) {
    ptr = add_song_to_tail(ptr, ptr->title, ptr->artist);
    ptr = remove_song_from_head(ptr);
  }

  return ptr;
} /* move_to_tail() */

/*
 * Define interleave_songs here:
 * A function that compares the sizes of two lists and interleaves the song
 * from the smaller list into the larger list, or the first into the second
 * if the same size. returns the header of the new list
 */

song_t *interleave_songs(song_t *first_ptr, song_t *second_ptr) {
  assert(first_ptr != NULL);
  assert(second_ptr != NULL);

  int first_size = count_songs(first_ptr);
  int second_size = count_songs(second_ptr);
  printf("first size = %d\n", first_size);
  printf("second size = %d\n", second_size);

  song_t *final_head = NULL;
  song_t *final_tail = NULL;
  if (first_size >= second_size) {
    final_head = first_ptr;
    final_tail = first_ptr;
    first_ptr = first_ptr->next_song;
    while ((first_ptr != NULL) || (second_ptr != NULL)) {
      if (second_ptr != NULL) {
        final_tail->next_song = second_ptr;
        final_tail = final_tail->next_song;
        second_ptr = second_ptr->next_song;
      }

      if (first_ptr != NULL) {
        final_tail->next_song = first_ptr;
        final_tail = final_tail->next_song;
        first_ptr = first_ptr->next_song;
      }
    }
  }
  else if (first_size < second_size) {
    printf("in else\n");
    final_head = second_ptr;
    final_tail = second_ptr;
    second_ptr = second_ptr->next_song;
    while ((first_ptr != NULL) || (second_ptr != NULL)) {
      if (first_ptr != NULL) {
        final_tail->next_song = first_ptr;
        final_tail = final_tail->next_song;
        first_ptr = first_ptr->next_song;
      }
      if (second_ptr != NULL) {
        final_tail->next_song = second_ptr;
        final_tail = final_tail->next_song;
        second_ptr = second_ptr->next_song;
      }


    }

  }

  return final_head;
} /* interleave_songs() */

/*
 * Define free_song_list here:
 * A function that goes through the given list and deallocates it all
 */

void free_song_list(song_t *ptr) {
  int num_songs = count_songs(ptr);
  song_t *store_ptr = NULL;

  for (int i = 0; i < num_songs; i++) {
    store_ptr = ptr->next_song;
    free(ptr->title);
    ptr->title = NULL;
    free(ptr->artist);
    ptr->artist = NULL;
    free(ptr);
    ptr = NULL;
    ptr = store_ptr;
  }

} /* free_song_list() */
