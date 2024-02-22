#ifndef HW6_H
#define HW6_H

/* Constant definitions */

#define MAX_FILE_LEN  (50)
#define MAX_PLAYERS   (20)
#define MAX_NAME_LEN  (20)
#define KEY_LEN       (20)
#define MAX_BUFF_LEN  (200)
#define MAX_FRIENDS   (3)

typedef struct player {
    char username[MAX_NAME_LEN];
    char secret[KEY_LEN];
    char friends[MAX_FRIENDS][MAX_NAME_LEN];
    int trust[MAX_FRIENDS];
} player_t;

/* Error codes */

#define NON_READABLE_FILE  (-1)
#define BAD_RECORD         (-2)
#define TOO_MUCH_DATA      (-3)
#define NO_PLAYERS         (-4)
#define NOT_FOUND          (-5)

/* Function prototypes */

int read_players(char *);
int find_imposters();

/* Global variables */

extern int g_player_count;
extern player_t g_player_array[MAX_PLAYERS];

#endif // HW6_H