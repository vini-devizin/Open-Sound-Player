#ifndef MUSIC_LIB_H
#define MUSIC_LIB_H

#include <dirent.h>

int alpha_sort(const void *a, const void *b);

void read_musics(char ***musics, DIR *dir, size_t *capacity, size_t *count);

void list_musics(char **musics, size_t count);

#endif
