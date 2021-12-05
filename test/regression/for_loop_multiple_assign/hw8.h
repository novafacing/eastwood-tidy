#ifndef HW8_H
#define HW8_H

/* Structure Declarations */

typedef struct song {
  char *title;
  char *artist;

  struct song *next_song;
} song_t;

/* Function prototypes */

song_t *add_song_to_head(song_t*, char *, char *);
song_t *remove_song_from_head(song_t *);
song_t *add_song_to_tail(song_t *, char *, char *);
song_t *remove_song_from_tail(song_t *);
int count_songs(song_t *);
song_t *search_by_index(song_t *, int);
song_t *search_by_title_and_artist(song_t *, char *, char *);
song_t *modify_song_by_index(song_t *, int, char *, char *);
song_t *move_to_tail(song_t *, int);
song_t *interleave_songs(song_t *, song_t *);
void free_song_list(song_t *);

#endif // HW8_H
