/* Name, hw8.c, CS 24000, Fall 2021
 * Last updated Oct 17, 2021
 */

#include "hw8.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

/*
 * adds song to the head of linked list
 */

song_t *add_song_to_head(song_t *head, char *title, char *artist) {

  assert((title != NULL) && (artist != NULL));

  struct song *new_head = malloc(sizeof(struct song));

  assert(new_head != NULL);

  new_head->title = malloc(strlen(title) + 1);
  new_head->artist = malloc(strlen(artist) + 1);

  assert((new_head->title != NULL) && (new_head->artist != NULL));

  strcpy(new_head->title, title);

  strcpy(new_head->artist, artist);

  new_head->title = title;
  new_head->artist = artist;

  new_head->next_song = head;

  return new_head;
} /* add_song_to_head() */

/*
 * removes the song at the head of the linked list
 */

song_t *remove_song_from_head(song_t *old_head) {

  if (old_head == NULL) {
    return (old_head);
  }

  struct song *new_head = (*old_head).next_song;

  free(old_head->title);
  old_head->title = NULL;

  free(old_head->artist);
  old_head->artist = NULL;

  free(old_head);

  old_head = NULL;

  return new_head;
} /* remove_song_from_head() */

/*
 * adds song to the tail of the linked list
 */

song_t *add_song_to_tail(song_t *head, char *title, char *artist) {

  assert((title != NULL) && (artist != NULL));

  struct song *new_tail = malloc(sizeof(struct song));

  assert(new_tail != NULL);

  new_tail->title = malloc(strlen(title) + 1);
  new_tail->artist = malloc(strlen(artist) + 1);

  assert((new_tail->title != NULL) && (new_tail->artist != NULL));

  strcpy(new_tail->title, title);

  strcpy(new_tail->artist, artist);

  new_tail->title = title;
  new_tail->artist = artist;
  new_tail->next_song = NULL;


  if (head == NULL) {
    head = new_tail;
    return (head);
  }

  struct song *previous_song = head;

  while ((previous_song->next_song != NULL)) {
    previous_song = previous_song->next_song;
  }

  previous_song->next_song = new_tail;

  return head;
} /* add_song_to_tail() */

/*
 * removes the song at the tail of the linked list
 */

song_t *remove_song_from_tail(song_t *head) {

  if (count_songs(head) <= 1) {
    return (remove_song_from_head(head));
  }

  struct song *new_tail = head;

  while (new_tail->next_song != NULL) {
    if ((new_tail->next_song)->next_song != NULL) {
      new_tail = new_tail->next_song;
    }
    else {
      break;
    }
  }

  struct song *old_tail = NULL;

  if (head->next_song != NULL) {
    old_tail = new_tail->next_song;
    new_tail->next_song = NULL;
  }
  else {
    old_tail = head;
  }

  free(old_tail->title);
  old_tail->title = NULL;

  free(old_tail->artist);
  old_tail->artist = NULL;

  free(old_tail);

  old_tail = NULL;

  return head;
} /* remove_song_from_tail() */

/*
 * returns then number of elements in the linked list
 */

int count_songs(song_t *head) {

  if (head == NULL) {
    return 0;
  }

  int size = 1;

  while (head->next_song != NULL) {
    head = head->next_song;
    size++;
  }

  return (size);
} /* count_songs() */

/*
 * returns the song at the specified index (1 indexed)
 */

song_t *search_by_index(song_t *head, int search_index) {

  assert(search_index > 0);

  if (count_songs(head) < search_index) {
    return (NULL);
  }

  for (int i = 1; i < search_index; i++) {
    head = head->next_song;
  }

  return (head);
} /* search_by_index() */

/*
 * finds the song with the title and artist
 */

song_t *search_by_title_and_artist(song_t *head, char *title, char *artist) {

  assert((title != NULL) && (artist != NULL));

  int size = count_songs(head);

  for (int i = 1; i <= size; i++) {
    printf("%s\n", head->title);
    if ((strcmp(title, head->title) == 0) &&
        (strcmp(artist, head->artist) == 0)) {
      return (head);
    }
    head = head->next_song;
  }

  return NULL;
} /* search_by_title_and_artist() */

/*
 * edits the song at the index
 */

song_t *modify_song_by_index(song_t *head, int editing_index, char *title,
                             char *artist) {

  assert(editing_index > 0);
  assert((title != NULL) && (artist != NULL));

  struct song *target = search_by_index(head, editing_index);

  if (target == NULL) {
    return (target);
  }

  free(target->title);
  target->title = NULL;

  free(target->artist);
  target->artist = NULL;

  target->title = malloc(strlen(title) + 1);
  assert(target->title != NULL);

  target->artist = malloc(strlen(artist) + 1);
  assert(target->artist != NULL);

  strcpy(target->title, title);
  strcpy(target->artist, artist);

  return target;
} /* modify_song_by_index() */

/*
 * moves the song to the tail
 */

song_t *move_to_tail(song_t *head, int number_to_move) {

  assert((number_to_move >= 0) && (number_to_move <= count_songs(head)));

  if (number_to_move == count_songs(head)) {
    return (head);
  }

  struct song *new_head = search_by_index(head, number_to_move + 1);

  search_by_index(head, number_to_move)->next_song = NULL;

  search_by_index(new_head, count_songs(new_head))->next_song = head;

  return new_head;
} /* move_to_tail() */

/*
 * interleaves the songs from the specified lists
 */

song_t *interleave_songs(song_t *list_1, song_t *list_2) {

  assert((list_1 != NULL) && (list_2 != NULL));

  struct song *larger = NULL;
  struct song *smaller = NULL;

  if (count_songs(list_1) >= count_songs(list_2)) {
    larger = list_1;
    smaller = list_2;
  }
  else {
    larger = list_2;
    smaller = list_1;
  }

  int size = count_songs(smaller);

  struct song *larger_temp = larger;
  struct song *smaller_temp = smaller;

  for (int i = 1; i <= size * 2; i += 2) {
    if (larger_temp != NULL) {
      larger_temp = larger_temp->next_song;
    }
    else {
      break;
    }
    search_by_index(larger, i)->next_song = smaller_temp;
    smaller_temp = smaller_temp->next_song;
    search_by_index(larger, i + 1)->next_song = larger_temp;
  }
  printf("%p\n", larger_temp);

  return larger;
} /* interleave_songs() */

/*
 * deletes the entire list
 */

void free_song_list(song_t *head) {

  while (count_songs(head) > 0) {
    head = remove_song_from_tail(head);
  }

} /* free_song_list() */


