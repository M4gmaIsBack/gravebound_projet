#ifndef TIME_H
#define TIME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "config.h"

extern Config config;

typedef struct {
    int hour;
    int minute;
    int second;
    int elapsed_time;
    float time;
    float OFFSET;
} time;

int init_time(time *countdown, char *save);
void update_time(time *countdown);
void display_time(time *countdown);
void enregistrer_time(time *countdown, char *save);
void skip_day(time *countdown);

#endif