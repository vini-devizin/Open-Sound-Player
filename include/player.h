#ifndef PLAYER_H
#define PLAYER_H

#include "miniaudio.h"

typedef struct Player Player;

Player *create_player();

void free_player(Player *player);

void start_sound(Player *player, int index, char *music, char **musics);

void next_sound(Player *player, int *index, size_t length, char *music, char **musics);

int handle_start_sound(Player *player, int index, size_t length, char *music, char **musics);

int sound_end(Player *player);

void stop_sound(Player *player);

void resume_sound(Player *player);

void loop_sound(Player *player);

void noloop_sound(Player *player);

#endif
