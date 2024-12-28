#include <stdio.h>
#include <stdlib.h>
#include "../logs/logging.h"
#include "../game/time.h"
#include "../UI/graphique.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int init_time(time *countdown, time temps) {
    countdown->hour = temps.hour;
    countdown->minute = temps.minute;
    countdown->second = temps.second;
    return 1;
}

void update_time(time *countdown) {
    countdown->second--;
    if (countdown->second <= 0) {
        countdown->second = 59;
        countdown->minute--;
        if (countdown->minute <= 0) {
            countdown->minute = 59;
            countdown->hour--;
            if (countdown->hour <= 0) {
                countdown->hour = 24;
            }
        }
    }
}

void display_time(time *countdown) {
    printf("Temps restant: %d:%d:%d\n", countdown->hour, countdown->minute, countdown->second);
}