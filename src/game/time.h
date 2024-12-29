#ifndef TIME_H
#define TIME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    int hour;
    int minute;
    int second;
    int elapsed_time;
} time;

int init_time(time *countdown, time set_time);
void update_time(time *countdown);
void display_time(time *countdown);

#endif