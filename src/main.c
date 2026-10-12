#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

int alpha_sort(const void *a, const void *b) {
  const char *music_a = *(const char **)a;
  const char *music_b = *(const char **)b;

  return strcmp(music_a, music_b);
}

void start_sound(ma_engine *engine, ma_sound *sound, int index, char *music, char **musics) {
  snprintf(music, 100, "Musics/%s", musics[index]);
  printf("Tocando agora: %s\n", musics[index]);

  ma_result result = ma_sound_init_from_file(engine, music, 0, NULL, NULL, sound);
  if (result != MA_SUCCESS) {
    printf("Erro ao inicar som: %s\n", ma_result_description(result));
    return;
  }
  ma_sound_start(sound);
}

int main() {
  struct pollfd pfd = {
    .fd = STDIN_FILENO,
    .events = POLLIN
  };
  DIR *dir = opendir("Musics");
  ma_result result;
  ma_engine engine;
  ma_sound sound;

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

  char music[100];
  char cmd[50];
  int opt;
  printf("Escolha a música\n> ");
  scanf("%d", &opt);
  getchar();
  start_sound(&engine, &sound, opt - 1, music, musics);
  printf("> ");
  fflush(stdout);

  while (1) {
    int poll_res = poll(&pfd, 1, 500);

    if (poll_res < 0) {
      perror("poll");
      return 1;
    }

    if (poll_res == 0) {
      if (ma_sound_at_end(&sound)) {
        ma_sound_uninit(&sound);
        if ((size_t)opt - 1 >= count - 1) {
          opt = 1;
        } else {
          opt++;
        }
        start_sound(&engine, &sound, opt - 1, music, musics);
      }
    }

    if (poll_res > 0 && (pfd.revents & POLLIN)) {
      fgets(cmd, sizeof(cmd), stdin);
      cmd[strcspn(cmd, "\n")] = '\0';

      if (strcmp(cmd, "quit") == 0) {
        break;
      } else if (strcmp(cmd, "pause") == 0) {
        ma_sound_stop(&sound);
      } else if (strcmp(cmd, "resume") == 0) {
        ma_sound_start(&sound);
      } else if (strcmp(cmd, "loop") == 0) {
        ma_sound_set_looping(&sound, MA_TRUE);
      } else if (strcmp(cmd, "noloop") == 0) {
        ma_sound_set_looping(&sound, MA_FALSE);
      }
      printf("> ");
    }
  }

  ma_sound_uninit(&sound);
  ma_engine_uninit(&engine);
  closedir(dir);
  for (size_t i = 0; i < count; i++) {
    free(musics[i]);
  }
  free(musics);

  return 0;
}
