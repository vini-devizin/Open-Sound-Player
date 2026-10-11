#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

int alpha_sort(const void *a, const void *b) {
  const char *music_a = *(const char **)a;
  const char *music_b = *(const char **)b;

  return strcmp(music_a, music_b);
}

int main() {
  DIR *dir = opendir("Musics");
  ma_result result;
  ma_engine engine;

  if (dir == NULL) {
    printf("Erro ao abrir diretório\n");
    return 1;
  }

  struct dirent *entry;
  size_t count = 0;
  size_t capacity = 4;
  char **musics = malloc(capacity * sizeof(*musics));

  printf("Open Player\n");

  while ((entry = readdir(dir)) != NULL) {
    size_t length = strlen(entry->d_name);
    if (length > 4 && strcmp(entry->d_name + length - 4, ".mp3") == 0) {
      if (count == capacity) {
        capacity *= 2;
        char **tmp = realloc(musics, capacity * sizeof(*musics));

        if (tmp == NULL) {
          printf("Erro ao realocar\n");
        } else {
          musics = tmp;
        }
      }
      
      musics[count] = strdup(entry->d_name);

      if (musics[count] == NULL) {
        printf("Erro ao alocar\n");
      } else {
        count++;
      }
    }
  }

  qsort(musics, count, sizeof(*musics), alpha_sort);

  for (size_t i = 0; i < count; i++) {
    printf("%zu - %s\n", i + 1, musics[i]);
  }

  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    return -1;
  }

  char music[100] = "Musics/";
  int opt;
  printf("Escolha a música\n> ");
  scanf("%d", &opt);
  getchar();
  strcat(music, musics[opt - 1]);

  ma_engine_play_sound(&engine, music, NULL);

  printf("Press Enter to quit... \n");
  getchar();

  ma_engine_uninit(&engine);
  closedir(dir);
  for (size_t i = 0; i < count; i++) {
    free(musics[i]);
  }
  free(musics);

  return 0;
}
