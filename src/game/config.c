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
            printf("Erreur ouverture fichier de config zombies\n");
            return 0;
        }
    }

    char buffer[4096]; 
    size_t read_size = fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    buffer[read_size] = '\0';
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

    char buffer[4096]; // Increase buffer size
    size_t read_size = fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    buffer[read_size] = '\0'; // Null-terminate the buffer
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

    char buffer[4096]; // Increase buffer size
    size_t read_size = fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    buffer[read_size] = '\0'; // Null-terminate the buffer
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

    char buffer[4096]; // Increase buffer size
    size_t read_size = fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    buffer[read_size] = '\0'; // Null-terminate the buffer
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

int enregistrer_config(char *save) {
    char filepath_zombie[100];
    char filepath_map[100];
    char filepath_personnage[100];
    char filepath_skills[100];

    snprintf(filepath_zombie, sizeof(filepath_zombie), "./saves/%s/config/zombies.json", save);
    snprintf(filepath_map, sizeof(filepath_map), "./saves/%s/config/map.json", save);
    snprintf(filepath_personnage, sizeof(filepath_personnage), "./saves/%s/config/personnage.json", save);
    snprintf(filepath_skills, sizeof(filepath_skills), "./saves/%s/config/skills.json", save);

    FILE *check_file_zombie = fopen(filepath_zombie, "r");
    FILE *check_file_map = fopen(filepath_map, "r");
    FILE *check_file_personnage = fopen(filepath_personnage, "r");
    FILE *check_file_skills = fopen(filepath_skills, "r");

    if (check_file_zombie != NULL && check_file_map != NULL && check_file_personnage != NULL && check_file_skills != NULL) {
        printf("Fichier de configuration déjà présent\n");

        fclose(check_file_zombie);
        fclose(check_file_map);
        fclose(check_file_personnage);
        fclose(check_file_skills);

        return 1;
    }

    if (check_file_zombie != NULL) fclose(check_file_zombie);
    if (check_file_map != NULL) fclose(check_file_map);
    if (check_file_personnage != NULL) fclose(check_file_personnage);
    if (check_file_skills != NULL) fclose(check_file_skills);

    FILE *file_zombie = fopen(filepath_zombie, "w");
    FILE *file_map = fopen(filepath_map, "w");
    FILE *file_personnage = fopen(filepath_personnage, "w");
    FILE *file_skills = fopen(filepath_skills, "w");

    if (file_zombie == NULL || file_map == NULL || file_personnage == NULL || file_skills == NULL) {
        printf("Erreur ouverture fichier config\n");
        return 0;
    }

    cJSON *json_zombie = cJSON_CreateObject();
    cJSON *json_map = cJSON_CreateObject();
    cJSON *json_personnage = cJSON_CreateObject();
    cJSON *json_skills = cJSON_CreateObject();

    cJSON *max_zombies = cJSON_CreateArray();
    cJSON *spawn_rate = cJSON_CreateArray();
    cJSON *spawn_radius = cJSON_CreateArray();
    cJSON *spawn_delay = cJSON_CreateArray();
    cJSON *max_speed = cJSON_CreateArray();
    cJSON *max_health = cJSON_CreateArray();
    cJSON *max_damage = cJSON_CreateArray();
    cJSON *max_defense = cJSON_CreateArray();

    cJSON *name = cJSON_CreateArray();
    cJSON *level = cJSON_CreateArray();
    cJSON *range = cJSON_CreateArray();
    cJSON *maxExperience = cJSON_CreateArray();
    cJSON *cooldown = cJSON_CreateArray();
    cJSON *lastUsed = cJSON_CreateArray();
    cJSON *damage = cJSON_CreateArray();
    
    for (int i = 0; i < 100; i++) {
        cJSON_AddItemToArray(max_zombies, cJSON_CreateNumber(config.zombies.max_zombies[i]));
        cJSON_AddItemToArray(spawn_rate, cJSON_CreateNumber(config.zombies.spawn_rate[i]));
        cJSON_AddItemToArray(spawn_radius, cJSON_CreateNumber(config.zombies.spawn_radius[i]));
        cJSON_AddItemToArray(spawn_delay, cJSON_CreateNumber(config.zombies.spawn_delay[i]));
        cJSON_AddItemToArray(max_speed, cJSON_CreateNumber(config.zombies.max_speed[i]));
        cJSON_AddItemToArray(max_health, cJSON_CreateNumber(config.zombies.max_health[i]));
        cJSON_AddItemToArray(max_damage, cJSON_CreateNumber(config.zombies.max_damage[i]));
        cJSON_AddItemToArray(max_defense, cJSON_CreateNumber(config.zombies.max_defense[i]));
    }

    for (int i = 0; i < 100; i++) {
        cJSON_AddItemToArray(name, cJSON_CreateString(config.skills.name[i]));
        cJSON_AddItemToArray(level, cJSON_CreateNumber(config.skills.level[i]));
        cJSON_AddItemToArray(range, cJSON_CreateNumber(config.skills.range[i]));
        cJSON_AddItemToArray(maxExperience, cJSON_CreateNumber(config.skills.maxExperience[i]));
        cJSON_AddItemToArray(cooldown, cJSON_CreateNumber(config.skills.cooldown[i]));
        cJSON_AddItemToArray(lastUsed, cJSON_CreateNumber(config.skills.lastUsed[i]));
        cJSON_AddItemToArray(damage, cJSON_CreateNumber(config.skills.damage[i]));
    }

    cJSON_AddItemToObject(json_zombie, "max_zombies", max_zombies);
    cJSON_AddItemToObject(json_zombie, "spawn_rate", spawn_rate);
    cJSON_AddItemToObject(json_zombie, "spawn_radius", spawn_radius);
    cJSON_AddItemToObject(json_zombie, "spawn_delay", spawn_delay);
    cJSON_AddItemToObject(json_zombie, "max_speed", max_speed);
    cJSON_AddItemToObject(json_zombie, "max_health", max_health);
    cJSON_AddItemToObject(json_zombie, "max_damage", max_damage);
    cJSON_AddItemToObject(json_zombie, "max_defense", max_defense);
    
    cJSON_AddItemToObject(json_skills, "name", name);
    cJSON_AddItemToObject(json_skills, "level", level);
    cJSON_AddItemToObject(json_skills, "range", range);
    cJSON_AddItemToObject(json_skills, "maxExperience", maxExperience);
    cJSON_AddItemToObject(json_skills, "cooldown", cooldown);
    cJSON_AddItemToObject(json_skills, "lastUsed", lastUsed);
    cJSON_AddItemToObject(json_skills, "damage", damage);

    cJSON_AddNumberToObject(json_map, "map_size", config.map.map_size);
    cJSON_AddStringToObject(json_map, "region", config.map.region);
    cJSON_AddNumberToObject(json_map, "scale", config.map.scale);
    cJSON_AddNumberToObject(json_map, "amplitude", config.map.amplitude);
    cJSON_AddNumberToObject(json_map, "start_countdown_hour", config.map.start_countdown_hour);
    cJSON_AddNumberToObject(json_map, "start_countdown_minute", config.map.start_countdown_minute);
    cJSON_AddNumberToObject(json_map, "start_countdown_second", config.map.start_countdown_second);
    cJSON_AddNumberToObject(json_map, "start_hour", config.map.start_hour);

    cJSON_AddItemToObject(json_personnage, "vie_max", cJSON_CreateNumber(*config.personnage.vie_max));
    cJSON_AddItemToObject(json_personnage, "force_attaque_max", cJSON_CreateNumber(*config.personnage.force_attaque_max));
    cJSON_AddItemToObject(json_personnage, "vitesse_max", cJSON_CreateNumber(*config.personnage.vitesse_max));
    cJSON_AddItemToObject(json_personnage, "defense_max", cJSON_CreateNumber(*config.personnage.defense_max));
    cJSON_AddItemToObject(json_personnage, "max_skills", cJSON_CreateNumber(*config.personnage.max_skills));

    cJSON_AddItemToObject(json_personnage, "skills", json_skills);

    // Write JSON objects to files
    char *json_zombie_string = cJSON_Print(json_zombie);
    char *json_map_string = cJSON_Print(json_map);
    char *json_personnage_string = cJSON_Print(json_personnage);
    char *json_skills_string = cJSON_Print(json_skills);

    fprintf(file_zombie, "%s", json_zombie_string);
    fprintf(file_map, "%s", json_map_string);
    fprintf(file_personnage, "%s", json_personnage_string);
    fprintf(file_skills, "%s", json_skills_string);

    fclose(file_zombie);
    fclose(file_map);
    fclose(file_personnage);
    fclose(file_skills);

    logMessage("Enregistrement de la configuration ok");

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

    if(!enregistrer_config(save)) {
        printf("Erreur enregistrement config\n");
        return 0;
    }

    return 1;
}

