#include <stdio.h>
#include <stdlib.h>
#include "../logs/logging.h"
#include "../game/time.h"
#include "../UI/graphique.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cjson/cJSON.h>

int init_time(time *countdown, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/state.json", save);
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        countdown->hour = config.map.start_countdown_hour;
        countdown->minute = config.map.start_countdown_minute;
        countdown->second = config.map.start_countdown_second;
        countdown->elapsed_time = 0;
        countdown->OFFSET = config.map.start_hour;
        return 0;
    }

    char buffer[1024];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        countdown->hour = config.map.start_countdown_hour;
        countdown->minute = config.map.start_countdown_minute;
        countdown->second = config.map.start_countdown_second;
        countdown->elapsed_time = 0;
        countdown->OFFSET = config.map.start_hour;
        return 0;
    }

    cJSON *elapsed_time = cJSON_GetObjectItem(json, "elapsed_time");
    cJSON *hour = cJSON_GetObjectItem(json, "hour");
    cJSON *minute = cJSON_GetObjectItem(json, "minute");
    cJSON *second = cJSON_GetObjectItem(json, "second");
    cJSON *time = cJSON_GetObjectItem(json, "time");
    cJSON *OFFSET = cJSON_GetObjectItem(json, "OFFSET");

    if (elapsed_time && hour && minute && second && time && OFFSET) {
        countdown->elapsed_time = elapsed_time->valueint;
        countdown->hour = hour->valueint;
        countdown->minute = minute->valueint;
        countdown->second = second->valueint;
        countdown->time = time->valuedouble;
        countdown->OFFSET = OFFSET->valuedouble;
    } else {
        countdown->hour = config.map.start_countdown_hour;
        countdown->minute = config.map.start_countdown_minute;
        countdown->second = config.map.start_countdown_second;
        countdown->elapsed_time = 0;
        countdown->OFFSET = config.map.start_hour;
    }

    cJSON_Delete(json);
    return 1;
}

void update_time(time *countdown) {
    countdown->second--;
    countdown->elapsed_time += 2;
    countdown->time = fmod(countdown->elapsed_time / 60.0, 24.0) + countdown->OFFSET;
    if (countdown->second <= 0 && countdown->minute > 0) {
        countdown->second = 59;
        countdown->minute--;
        if (countdown->minute <= 0 && countdown->hour > 0) {
            countdown->minute = 59;
            countdown->hour--;
            if (countdown->hour <= 0) {
                countdown->hour = 0;
            }
        }
    } else if (countdown->second <= 0 && countdown->minute <= 0) {
        countdown->second = 0;
        countdown->minute = 0;
    }
}

void enregistrer_time(time *countdown, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/state.json", save);
    FILE *file = fopen(filepath, "w");
    if (file) {
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "elapsed_time", countdown->elapsed_time);
        cJSON_AddNumberToObject(json, "hour", countdown->hour);
        cJSON_AddNumberToObject(json, "minute", countdown->minute);
        cJSON_AddNumberToObject(json, "second", countdown->second);
        cJSON_AddNumberToObject(json, "time", countdown->time);
        cJSON_AddNumberToObject(json, "OFFSET", countdown->OFFSET);

        char *json_string = cJSON_Print(json);
        fprintf(file, "%s\n", json_string);

        free(json_string);
        cJSON_Delete(json);
        fclose(file);
    } else {
        logMessage("Erreur ouverture fichier state.json");
    }
}

void display_time(time *countdown) {
    printf("Temps restant: %d:%d:%d\n", countdown->hour, countdown->minute, countdown->second);
    printf("Temps écoulé: %d\n", countdown->elapsed_time);
    printf("heure: %f\n", countdown->time);
}

void skip_day(time *countdown) {
    if(countdown->time > 8 && countdown->time < 18) {
        float diff = 18 - countdown->time;
        printf("diff: %f\n", diff);
        countdown->elapsed_time += diff * 60;
    } else {
        printf("nuh uh\n");
        printf("time: %f\n", countdown->time);
    }
}