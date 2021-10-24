#ifndef HW7_H
#define HW7_H

/* Constant definitions */
#define MAX_FILE_LEN       (50)
#define MAX_SONGS          (50)
#define MAX_NAME_LEN       (50)

typedef struct song {
  char song_name[MAX_NAME_LEN];
  char singer_name[MAX_NAME_LEN];
  char genre_name[MAX_NAME_LEN];
  char cover_artist[MAX_NAME_LEN];

  struct song *next_cover_ptr;
  struct song *next_song_ptr;
} song_t;

/* Error codes */
#define SONG_FOUND          (0)
#define NON_READABLE_FILE  (-1)
#define BAD_RECORD         (-2)
#define OUT_OF_BOUND       (-3)
#define NO_SONGS           (-4)
#define SONG_NOT_FOUND     (-5)

/* Function prototypes */
int read_songs(char *);
void link_songs();
int find_song_with_singer_name(int, char *);
int count_song_cover_links(int);
int find_songs_by_genre_name(char *, int *);

/* Global variables */
extern int g_song_count;
extern song_t g_song_array[MAX_SONGS];

#endif // HW7_H

