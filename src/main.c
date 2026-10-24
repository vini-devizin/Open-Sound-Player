#include <stdio.h>
#include <stdlib.h>
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

  if (dir == NULL) {
    printf("Erro ao abrir diretório\n");
    return 1;
  }

  size_t count = 0;
  size_t capacity = 4;
  char **musics = malloc(capacity * sizeof(*musics));

  printf("Open Sound Player\n");

  read_musics(&musics, dir, &capacity, &count);

  qsort(musics, count, sizeof(*musics), alpha_sort);

  list_musics(musics, count);

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
        next_sound(&engine, &sound, &opt, count, music, musics);
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
        next_sound(&engine, &sound, &opt, count, music, musics);
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
