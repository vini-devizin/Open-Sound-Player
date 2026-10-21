#include <stdio.h>
//#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

void start_sound(ma_engine *engine, ma_sound *sound, int index, char *music, char **musics) {
  snprintf(music, 100, "Musics/%s", musics[index]);
  printf("\n> Tocando agora: %s\n", musics[index]);

  ma_result result = ma_sound_init_from_file(engine, music, 0, NULL, NULL, sound);
  if (result != MA_SUCCESS) {
    printf("Erro ao inicar som: %s\n", ma_result_description(result));
    return;
  }
  ma_sound_start(sound);
}
