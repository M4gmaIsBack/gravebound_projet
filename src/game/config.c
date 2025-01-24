#include "config.h"
#include <cjson/cJSON.h>

Config config;

int charger_config_zombies(Config_values_zombies *zombies, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/zombies.json", save);
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        file = fopen("./config_default/zombies.json", "r");
        if (file == NULL) {
            printf("Erreur ouverture fichier zombies\n");
            return 0;
        }
    }

    char buffer[1024];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Erreur parsing fichier zombies.json\n");
        return 0;
    }

    cJSON *max_zombies = cJSON_GetObjectItem(json, "max_zombies");
    cJSON *spawn_rate = cJSON_GetObjectItem(json, "spawn_rate");
    cJSON *spawn_radius = cJSON_GetObjectItem(json, "spawn_radius");
    cJSON *spawn_delay = cJSON_GetObjectItem(json, "spawn_delay");
    cJSON *max_speed = cJSON_GetObjectItem(json, "max_speed");
    cJSON *max_health = cJSON_GetObjectItem(json, "max_health");
    cJSON *max_damage = cJSON_GetObjectItem(json, "max_damage");
    cJSON *max_defense = cJSON_GetObjectItem(json, "max_defense");

    if (max_zombies && spawn_rate && spawn_radius && spawn_delay && max_speed && max_health && max_damage && max_defense) {
        for (int i = 0; i < cJSON_GetArraySize(max_zombies); i++) {
            zombies->max_zombies[i] = cJSON_GetArrayItem(max_zombies, i)->valueint;
            zombies->spawn_rate[i] = cJSON_GetArrayItem(spawn_rate, i)->valueint;
            zombies->spawn_radius[i] = cJSON_GetArrayItem(spawn_radius, i)->valueint;
            zombies->spawn_delay[i] = cJSON_GetArrayItem(spawn_delay, i)->valueint;
            zombies->max_speed[i] = cJSON_GetArrayItem(max_speed, i)->valuedouble;
            zombies->max_health[i] = cJSON_GetArrayItem(max_health, i)->valuedouble;
            zombies->max_damage[i] = cJSON_GetArrayItem(max_damage, i)->valuedouble;
            zombies->max_defense[i] = cJSON_GetArrayItem(max_defense, i)->valuedouble;
        }
    } else {
        printf("Erreur lecture des paramètres zombies\n");
        cJSON_Delete(json);
        return 0;
    }

    cJSON_Delete(json);
    return 1;
}

int charger_config_map(Config_values_map *map, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/map.json", save);
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        file = fopen("./config_default/map.json", "r");
        if (file == NULL) {
            printf("Erreur ouverture fichier map\n");
            return 0;
        }
    }

    char buffer[1024];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Erreur parsing fichier map.json\n");
        return 0;
    }

    cJSON *map_size = cJSON_GetObjectItem(json, "map_size");
    cJSON *region = cJSON_GetObjectItem(json, "region");
    cJSON *scale = cJSON_GetObjectItem(json, "scale");
    cJSON *amplitude = cJSON_GetObjectItem(json, "amplitude");
    cJSON *start_countdown_hour = cJSON_GetObjectItem(json, "start_countdown_hour");
    cJSON *start_countdown_minute = cJSON_GetObjectItem(json, "start_countdown_minute");
    cJSON *start_countdown_second = cJSON_GetObjectItem(json, "start_countdown_second");
    cJSON *start_hour = cJSON_GetObjectItem(json, "start_hour");

    if (map_size && region && scale && amplitude && start_countdown_hour && start_countdown_minute && start_countdown_second && start_hour) {
        map->map_size = map_size->valueint;
        map->region = strdup(region->valuestring);
        map->scale = scale->valuedouble;
        map->amplitude = amplitude->valuedouble;
        map->start_countdown_hour = start_countdown_hour->valueint;
        map->start_countdown_minute = start_countdown_minute->valueint;
        map->start_countdown_second = start_countdown_second->valueint;
        map->start_hour = start_hour->valuedouble;
    } else {
        printf("Erreur lecture des paramètres map\n");
        cJSON_Delete(json);
        return 0;
    }

    cJSON_Delete(json);
    return 1;
}

int charger_config_personnage(Config_values_personnage *personnage, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/personnage.json", save);
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        file = fopen("./config_default/personnage.json", "r");
        if (file == NULL) {
            printf("Erreur ouverture fichier personnage\n");
            return 0;
        }
    }

    char buffer[1024];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Erreur parsing fichier personnage.json\n");
        return 0;
    }

    cJSON *vie_max = cJSON_GetObjectItem(json, "vie_max");
    cJSON *force_attaque_max = cJSON_GetObjectItem(json, "force_attaque_max");
    cJSON *vitesse_max = cJSON_GetObjectItem(json, "vitesse_max");
    cJSON *defense_max = cJSON_GetObjectItem(json, "defense_max");
    cJSON *max_skills = cJSON_GetObjectItem(json, "max_skills");

    if (vie_max && force_attaque_max && vitesse_max && defense_max && max_skills) {
        for (int i = 0; i < cJSON_GetArraySize(vie_max); i++) {
            personnage->vie_max[i] = cJSON_GetArrayItem(vie_max, i)->valuedouble;
            personnage->force_attaque_max[i] = cJSON_GetArrayItem(force_attaque_max, i)->valuedouble;
            personnage->vitesse_max[i] = cJSON_GetArrayItem(vitesse_max, i)->valuedouble;
            personnage->defense_max[i] = cJSON_GetArrayItem(defense_max, i)->valuedouble;
            personnage->max_skills[i] = cJSON_GetArrayItem(max_skills, i)->valueint;
        }
    } else {
        printf("Erreur lecture des paramètres personnage\n");
        cJSON_Delete(json);
        return 0;
    }

    cJSON_Delete(json);
    return 1;
}

int charger_config_skills(Config_values_skills *skill, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/skills.json", save);
    FILE *file = fopen(filepath, "r");

    if (file == NULL) {
        file = fopen("./config_default/skills.json", "r");
        if (file == NULL) {
            printf("Erreur ouverture fichier skills\n");
            return 0;
        }
    }

    char buffer[1024];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Erreur parsing fichier skills.json\n");
        return 0;
    }

    cJSON *name = cJSON_GetObjectItem(json, "name");
    cJSON *level = cJSON_GetObjectItem(json, "level");
    cJSON *range = cJSON_GetObjectItem(json, "range");
    cJSON *maxExperience = cJSON_GetObjectItem(json, "maxExperience");
    cJSON *cooldown = cJSON_GetObjectItem(json, "cooldown");
    cJSON *lastUsed = cJSON_GetObjectItem(json, "lastUsed");
    cJSON *damage = cJSON_GetObjectItem(json, "damage");

    if (name && level && range && maxExperience && cooldown && lastUsed && damage) {
        for (int i = 0; i < cJSON_GetArraySize(name); i++) {
            strcpy(skill->name[i], cJSON_GetArrayItem(name, i)->valuestring);
            skill->level[i] = cJSON_GetArrayItem(level, i)->valueint;
            skill->range[i] = cJSON_GetArrayItem(range, i)->valueint;
            skill->maxExperience[i] = cJSON_GetArrayItem(maxExperience, i)->valueint;
            skill->cooldown[i] = cJSON_GetArrayItem(cooldown, i)->valueint;
            skill->lastUsed[i] = cJSON_GetArrayItem(lastUsed, i)->valueint;
            skill->damage[i] = cJSON_GetArrayItem(damage, i)->valueint;
        }
    } else {
        printf("Erreur lecture des paramètres skills\n");
        cJSON_Delete(json);
        return 0;
    }

    cJSON_Delete(json);
    return 1;
}

int charger_config(char *save) {
    if (!charger_config_zombies(&config.zombies, save)) {
        printf("Erreur chargement config zombies\n");
        return 0;
    }
    if (!charger_config_map(&config.map, save)) {
        printf("Erreur chargement config map\n");
        return 0;
    }
    if (!charger_config_personnage(&config.personnage, save)) {
        printf("Erreur chargement config personnage\n");
        return 0;
    }
    if (!charger_config_skills(&config.skills, save)) {
        printf("Erreur chargement config skills\n");
        return 0;
    }
    return 1;
}