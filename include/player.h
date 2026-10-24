#ifndef PLAYER_H
#define PLAYER_H

void start_sound(ma_engine *engine, ma_sound *sound, int index, char *music, char **musics);

void stop_sound(ma_engine *engine, ma_sound *sound);

void next_sound(ma_engine *engine, ma_sound *sound, int *index, size_t length, char *music, char **musics);

#endif
