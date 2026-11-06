#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "player.h"

struct Player {
  ma_engine *engine;
  ma_sound *sound;
  bool sound_initialized;
};

Player *create_player() {
  Player *player = malloc(sizeof *player);

  if (player == NULL) {
    return NULL;
  }

  player->engine = malloc(sizeof *player->engine);
  player->sound = malloc(sizeof *player->sound);
  player->sound_initialized = false;

  if (player->engine == NULL || player->sound == NULL) {
    free(player->engine);
    free(player->sound);
    free(player);
    return NULL;
  }

  if (ma_engine_init(NULL, player->engine) != MA_SUCCESS) {
    free(player);
    return NULL;
  }

  return player;
}

void free_player(Player *player) {
  ma_sound_uninit(player->sound);
  ma_engine_uninit(player->engine);
  free(player->engine);
  free(player->sound);
  free(player);
}

void start_sound(Player *player, int index, char *music, char **musics) {
  snprintf(music, 100, "Musics/%s", musics[index]);
  printf("\n> Tocando agora: %s\n", musics[index]);

  ma_result result = ma_sound_init_from_file(player->engine, music, 0, NULL, NULL, player->sound);
  if (result != MA_SUCCESS) {
    printf("Erro ao inicar som: %s\n", ma_result_description(result));
    return;
  }
  player->sound_initialized = true;
  ma_sound_start(player->sound);
}

void next_sound(Player *player, int *index, size_t length, char *music, char **musics) {
  ma_sound_uninit(player->sound);
  if ((size_t)(*index) - 1 >= length - 1) {
    *index = 1;
  } else {
    (*index)++;
  }
  start_sound(player, *index - 1, music, musics);
}

int handle_start_sound(Player *player, int index, size_t length, char *music, char **musics) {
  if (index < 1 || (size_t)index > length) {
    printf("Valor fora do alcance\n");
    return 0;
  } else {
    if (player->sound_initialized) {
      ma_sound_uninit(player->sound);
      player->sound_initialized = false;
    }
    start_sound(player, index - 1, music, musics);
  }
  return 1;
}

int sound_end(Player *player) {
  return ma_sound_at_end(player->sound);
}

void stop_sound(Player *player) {
  ma_sound_stop(player->sound);
}

void resume_sound(Player *player) {
  ma_sound_start(player->sound);
}

void loop_sound(Player *player) {
  ma_sound_set_looping(player->sound, MA_TRUE);
}

void noloop_sound(Player *player) {
  ma_sound_set_looping(player->sound, MA_FALSE);
}
