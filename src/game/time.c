#include <stdio.h>
#include <stdlib.h>
#include "../logs/logging.h"
#include "../game/time.h"
#include "../UI/graphique.h"
#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int init_time(time *countdown, time temps, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/state.txt", save);
    FILE *file = fopen(filepath, "r");
    if (file == NULL || fscanf(file, "%d\n%d\n%d\n%d\n%f\n%f", &countdown->elapsed_time, &countdown->hour, &countdown->minute, &countdown->second, &countdown->time, &countdown->OFFSET) != 6) {
        countdown->hour = temps.hour;
        countdown->minute = temps.minute;
        countdown->second = temps.second;
        countdown->elapsed_time = temps.elapsed_time;
        countdown->OFFSET = temps.OFFSET;
    }
    if (file != NULL) {
        fclose(file);
    }
    return 1;
}

void update_time(time *countdown) {
    countdown->second--;
    countdown->elapsed_time++;
    countdown->time = fmod(countdown->elapsed_time/ 60.0, 24.0) + countdown->OFFSET;
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

void enregistrer_time(time *countdown, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/state.txt", save);
    FILE *file = fopen(filepath, "w");
    if (file) {
        fprintf(file, "%d\n", countdown->elapsed_time);
        fprintf(file, "%d\n", countdown->hour);
        fprintf(file, "%d\n", countdown->minute);
        fprintf(file, "%d\n", countdown->second);
        fprintf(file, "%f\n", countdown->time);
        fprintf(file, "%f\n", countdown->OFFSET);
        fclose(file);
    } else {
        logMessage("Erreur ouverture fichier state.txt");
    }
}

void display_time(time *countdown) {
    // printf("Temps restant: %d:%d:%d\n", countdown->hour, countdown->minute, countdown->second);
    // printf("Temps écoulé: %d\n", countdown->elapsed_time);
    // printf("heure: %f\n", countdown->time);
}