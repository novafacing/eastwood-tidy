#ifndef EMPTY_LINE_COMMENT_H
#define EMPTY_LINE_COMMENT_H

typedef struct song {
    char *title;
    char *artist;

    struct song *next_song;
} song_t;

#endif // EMPTY_LINE_COMMENT_H