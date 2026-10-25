#ifndef PLAYER_H
#define PLAYER_H

void start_sound(ma_engine *engine, ma_sound *sound, int index, char *music, char **musics, bool *sound_initialized);

void stop_sound(ma_engine *engine, ma_sound *sound);

void next_sound(ma_engine *engine, ma_sound *sound, int *index, size_t length, char *music, char **musics, bool *sound_initialized);

int handle_start_sound(ma_engine *engine, ma_sound *sound, int index, size_t length, char *music, char **musics, bool *sound_initialized);

#endif
