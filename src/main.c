#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include "player.h"
#include "music_lib.h"

int main() {
  struct pollfd pfd = {
    .fd = STDIN_FILENO,
    .events = POLLIN
  };
  Player *player = create_player();
  DIR *dir = opendir("Musics");
  size_t count = 0;
  size_t capacity = 4;
  char **musics = malloc(capacity * sizeof(*musics));
  char music[100];
  read_musics(&musics, dir, &capacity, &count);
  qsort(musics, count, sizeof(*musics), alpha_sort);
  char cmd[50];
  int opt;

  if (dir == NULL) {
    printf("Erro ao abrir diretório\n");
    return 1;
  }

  printf("Open Sound Player\n");
  printf("Escolha a música\n> ");
  while (1) {
    list_musics(musics, count);
    scanf("%49s", cmd);
    if (strtol(cmd, NULL, 10)) {
      if (handle_start_sound(player, strtol(cmd, NULL, 10), count, music, musics)) {
        opt = strtol(cmd, NULL, 10);
        break;
      }
    }
  }
  printf("> ");
  fflush(stdout);

  while (1) {
    int poll_res = poll(&pfd, 1, 500);

    if (poll_res < 0) {
      perror("poll");
      return 1;
    }

    if (poll_res == 0) {
      if (sound_end(player)) {
        next_sound(player, &opt, count, music, musics);
      }
    }

    if (poll_res > 0 && (pfd.revents & POLLIN)) {
      fgets(cmd, sizeof(cmd), stdin);
      cmd[strcspn(cmd, "\n")] = '\0';

      if (strcmp(cmd, "quit") == 0) {
        break;
      } else if (strcmp(cmd, "pause") == 0) {
        stop_sound(player);
      } else if (strcmp(cmd, "resume") == 0) {
        resume_sound(player);
      } else if (strcmp(cmd, "loop") == 0) {
        loop_sound(player);
      } else if (strcmp(cmd, "noloop") == 0) {
        noloop_sound(player);
      } else if (strcmp(cmd, "list") == 0) {
        list_musics(musics, count);
      } else if (strcmp(cmd, "next") == 0) {
        next_sound(player, &opt, count, music, musics);
      } else if (strncmp(cmd, "play", 4) == 0) {
        if (strtol(cmd + 5, NULL, 10)) {
          if (handle_start_sound(player, strtol(cmd + 5, NULL, 10), count, music, musics)) {
            opt = strtol(cmd + 5, NULL, 10);
          }
        }
      }
      printf("> ");
      fflush(stdout);
    }
  }

  free_player(player);
  closedir(dir);
  for (size_t i = 0; i < count; i++) {
    free(musics[i]);
  }
  free(musics);

  return 0;
}
