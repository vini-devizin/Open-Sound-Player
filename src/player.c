#include <stdio.h>
#include <stdbool.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

void start_sound(ma_engine *engine, ma_sound *sound, int index, char *music, char **musics, bool *sound_initialized) {
  snprintf(music, 100, "Musics/%s", musics[index]);
  printf("\n> Tocando agora: %s\n", musics[index]);

  ma_result result = ma_sound_init_from_file(engine, music, 0, NULL, NULL, sound);
  if (result != MA_SUCCESS) {
    printf("Erro ao inicar som: %s\n", ma_result_description(result));
    return;
  }
  *sound_initialized = true;
  ma_sound_start(sound);
}

void next_sound(ma_engine *engine, ma_sound *sound, int *index, size_t length, char *music, char **musics, bool *sound_initialized) {
  ma_sound_uninit(sound);
  if ((size_t)(*index) - 1 >= length - 1) {
    *index = 1;
  } else {
    (*index)++;
  }
  start_sound(engine, sound, *index - 1, music, musics, sound_initialized);
}

int handle_start_sound(ma_engine *engine, ma_sound *sound, int index, size_t length, char *music, char **musics, bool *sound_initialized) {
  if (index < 1 || (size_t)index > length) {
    printf("Valor fora do alcance\n");
    return 0;
  } else {
    if (*sound_initialized) {
      ma_sound_uninit(sound);
      *sound_initialized = false;
    }
    start_sound(engine, sound, index - 1, music, musics, sound_initialized);
  }
  return 1;
}
