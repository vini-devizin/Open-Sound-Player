#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int alpha_sort(const void *a, const void *b) {
  const char *music_a = *(const char **)a;
  const char *music_b = *(const char **)b;

  return strcmp(music_a, music_b);
}

void read_musics(char ***musics, DIR *dir, size_t *capacity, size_t *count) {
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    size_t length = strlen(entry->d_name);
    if (length > 4 && strcmp(entry->d_name + length - 4, ".mp3") == 0) {
      if (*count == *capacity) {
        *capacity *= 2;
        char **tmp = realloc(*musics, *capacity * sizeof(**musics));

        if (tmp == NULL) {
          printf("Erro ao realocar\n");
        } else {
          (*musics) = tmp;
        }
      }
      
      (*musics)[*count] = strdup(entry->d_name);

      if ((*musics)[*count] == NULL) {
        printf("Erro ao alocar\n");
      } else {
        (*count)++;
      }
    }
  }
}

void list_musics(char **musics, size_t count) {
  for (size_t i = 0; i < count; i++) {
    printf("%zu - %s\n", i + 1, musics[i]);
  }
}
