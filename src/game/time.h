#ifndef TIME_H
#define TIME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../game/game.h"

extern char* SAVE_FILE;

int init_time(time *countdown, time set_time, char *save);
void update_time(time *countdown);
void display_time(time *countdown);
void enregistrer_time(time *countdown, char *save);

#endif