#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include "miniaudio.h"
#include "player.h"
#include "music_lib.h"

int main() {
  struct pollfd pfd = {
    .fd = STDIN_FILENO,
    .events = POLLIN
  };
  DIR *dir = opendir("Musics");
  ma_result result;
  ma_engine engine;
  ma_sound sound;
  size_t count = 0;
  size_t capacity = 4;
  char **musics = malloc(capacity * sizeof(*musics));
  char music[100];
  read_musics(&musics, dir, &capacity, &count);
  qsort(musics, count, sizeof(*musics), alpha_sort);
  char cmd[50];
  int opt;
  bool sound_initialized = false;

  if (dir == NULL) {
    printf("Erro ao abrir diretório\n");
    return 1;
  }

  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    return -1;
  }

  printf("Open Sound Player\n");
  printf("Escolha a música\n> ");
  while (1) {
    list_musics(musics, count);
    scanf("%99s", cmd);
    if (strtol(cmd, NULL, 10)) {
      if (handle_start_sound(&engine, &sound, strtol(cmd, NULL, 10), count, music, musics, &sound_initialized)) {
        opt = strtol(cmd, NULL, 10);
        break;
      }
    }
  }
  /*scanf("%d", &opt);
  getchar();
  start_sound(&engine, &sound, opt - 1, music, musics);*/
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
        next_sound(&engine, &sound, &opt, count, music, musics, &sound_initialized);
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
      } else if (strcmp(cmd, "list") == 0) {
        list_musics(musics, count);
      } else if (strcmp(cmd, "next") == 0) {
        next_sound(&engine, &sound, &opt, count, music, musics, &sound_initialized);
      } else if (strncmp(cmd, "play", 4) == 0) {
        if (strtol(cmd + 5, NULL, 10)) {
          if (handle_start_sound(&engine, &sound, strtol(cmd + 5, NULL, 10), count, music, musics, &sound_initialized)) {
            opt = strtol(cmd + 5, NULL, 10);
          }
        }
      }
      printf("> ");
      fflush(stdout);
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
