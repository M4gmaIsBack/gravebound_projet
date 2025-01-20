#include "config.h"

Config config;

int charger_config_zombies(Config_values_zombies *zombies, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/zombies.txt", save);
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        file = fopen("./config_default/zombies.txt", "r");
        if (file == NULL) {
            printf("Erreur ouverture fichier zombies\n");
            return 0;
        }
    }

    int MAX_ZOMBIE_LEVELS = 0;
    if (fscanf(file, "%d", &MAX_ZOMBIE_LEVELS) != 1) {
        fclose(file);
        return 0;
    }

    for (int i = 0; i < MAX_ZOMBIE_LEVELS; i++) {
        if (fscanf(file, "%d %d %d %d %f %f %f %f", 
                   &zombies->max_zombies[i], 
                   &zombies->spawn_rate[i], 
                   &zombies->spawn_radius[i], 
                   &zombies->spawn_delay[i], 
                   &zombies->max_speed[i], 
                   &zombies->max_health[i], 
                   &zombies->max_damage[i], 
                   &zombies->max_defense[i]) != 8) {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

int charger_config_map(Config_values_map *map, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/map.txt", save);
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        file = fopen("./config_default/map.txt", "r");
        if (file == NULL) {
            printf("Erreur ouverture fichier map\n");
            return 0;
        }
    }

    char region[50];
    if (fscanf(file, "%d\n%49s\n%f\n%f\n%d\n%d\n%d\n%f", &map->map_size, region, &map->scale, &map->amplitude, &map->start_countdown_hour, &map->start_countdown_minute, &map->start_countdown_second, &map->start_hour) != 8) {
        fclose(file);
        return 0;
    }
    map->region = strdup(region); // Allocate and copy the region string

    fclose(file);
    return 1;
}

int charger_config_personnage(Config_values_personnage *personnage, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/personnage.txt", save);
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        file = fopen("./config_default/personnage.txt", "r");
        if (file == NULL) {
            printf("Erreur ouverture fichier personnage\n");
            return 0;
        }
    }

    int MAX_PERSONNAGE_LEVEL = 0;
    if (fscanf(file, "%d", &MAX_PERSONNAGE_LEVEL) != 1) {
        fclose(file);
        return 0;
    }
    for (int i = 0; i < MAX_PERSONNAGE_LEVEL; i++) {
        if (fscanf(file, "%f %f %f %f %d", 
                   &personnage->vie_max[i], 
                   &personnage->force_attaque_max[i], 
                   &personnage->vitesse_max[i], 
                   &personnage->defense_max[i], 
                   &personnage->max_skills[i]) != 5) {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

int charger_config_skills(Config_values_skills *skill, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/skills.txt", save);
    FILE *file = fopen(filepath, "r");

    if (file == NULL) {
        file = fopen("./config_default/skills.txt", "r");
        if (file == NULL) {
            printf("Erreur ouverture fichier skills\n");
            return 0;
        }
    }

    int MAX_SKILLS_LEVEL = 0;
    int MAX_SKILLS = 0;
    if (fscanf(file, "%d %d", &MAX_SKILLS_LEVEL, &MAX_SKILLS) != 2) {
        fclose(file);
        return 0;
    }

    for (int i = 0; i < MAX_SKILLS; i++) {
        for (int j = 0; j < MAX_SKILLS_LEVEL; j++) {
            if (fscanf(file, "%s %d %d %d %d %d %d", 
                    skill->name[i], 
                    &skill->level[i], 
                    &skill->range[i], 
                    &skill->maxExperience[i], 
                    &skill->cooldown[i], 
                    &skill->lastUsed[i], 
                    &skill->damage[i]) != 7) {
                fclose(file);
                return 0;
            }
        }
    }
    fclose(file);
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