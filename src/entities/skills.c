#include "skills.h"
#include "character.h"
#include "zombies.h"
#include <cjson/cJSON.h>

int skill_escape(Personnage *personnage, int x, int y, int slot_number) {       
    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;
        personnage->x = x;
        personnage->y = y;
        logMessage("Escape effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}


int skill_teleport(Personnage *personnage, int slot_number) {

    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;
        personnage->x = 0;
        personnage->y = 0;

        logMessage("Teleport effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}

int skill_knockback(Personnage *personnage, int slot_number, int x, int y) {
    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;


    

        for (int i = 0; i < nombre_zombies; i++) {
            int dx = zombies[i]->x - x;
            int dy = zombies[i]->y - y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance < personnage->skills[slot_number].range) {
                float knockback_x = (dx / distance) * personnage->skills[slot_number].range;
                float knockback_y = (dy / distance) * personnage->skills[slot_number].range;
                zombies[i]->x += knockback_x;
                zombies[i]->y += knockback_y;
            }
        }

        logMessage("Knockback effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}

int skill_fire_ball(Personnage * personnage, int slot_number, int x, int y) {
    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;

        int closest_zombie = -1;
        for (int i = 0; i < nombre_zombies; i++) {
            int dx = zombies[i]->x - x;
            int dy = zombies[i]->y - y;
            float distance = sqrt(dx * dx + dy * dy);
            
            if (distance < personnage->skills[slot_number].range) {
                if (closest_zombie == -1 || distance < sqrt((zombies[closest_zombie]->x - x) * (zombies[closest_zombie]->x - x) + (zombies[closest_zombie]->y - y) * (zombies[closest_zombie]->y - y))) {
                    closest_zombie = i;
                }
            }
        }

        if (closest_zombie != -1) {
            for (int i = 0; i < nombre_zombies; i++) {
                int dx = zombies[i]->x - zombies[closest_zombie]->x;
                int dy = zombies[i]->y - zombies[closest_zombie]->y;
                float distance = sqrt(dx * dx + dy * dy);
                if (distance < personnage->skills[slot_number].range / 3) {
                    zombies[i]->sante -= personnage->skills[slot_number].damage;
                    logMessage("Zombie touché, %d dégâts infligés", personnage->skills[slot_number].damage);
                }
            }
        }

        logMessage("Fire ball effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}

int skill_cage(Personnage *personnage, int slot_number, int x, int y) {
    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;


        for (int i = 0; i < nombre_zombies; i++) {
            int dx = zombies[i]->x - x;
            int dy = zombies[i]->y - y;

            float distance = sqrt(dx * dx + dy * dy);

            if (distance < personnage->skills[slot_number].range) {
                zombies[i]->sante -= personnage->skills[slot_number].damage;
                zombies[i]->vitesse *= 0.9 / personnage->skills[slot_number].level;
                logMessage("Zombie touché, %d dégâts infligés", personnage->skills[slot_number].damage);
            }
        }

        logMessage("Light flow effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}

int skill_light_speed(Personnage *personnage, int slot_number) {
    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;
        personnage->vitesse *= personnage->skills[slot_number].level * 0.1;
        logMessage("Light speed effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}

int skill_berzerk(Personnage *personnage, int slot_number) {
    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;
        personnage->force_attaque += personnage->skills[slot_number].level * 1.5;
        personnage->vitesse *= personnage->skills[slot_number].level * 0.05;
        personnage->defense = 0;
        logMessage("Berzerk effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}

int skill_kamikaze(Personnage *personnage, int slot_number, int x, int y) {
    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;
        personnage->vie_actuelle *= 0.01;

        for (int i = 0; i < nombre_zombies; i++) {
            float dx = zombies[i]->x - x;
            float dy = zombies[i]->y - y;

            float distance = sqrt(dx * dx + dy * dy);

            if (distance < personnage->skills[slot_number].range) {
                zombies[i]->sante -= personnage->skills[slot_number].damage;
                logMessage("Zombie touché, %d dégâts infligés", personnage->skills[slot_number].damage);
            }
        }


        logMessage("Kamikaze effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}

int skill_heal(Personnage *personnage, int slot_number) {
    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;
        personnage->vie_actuelle += personnage->skills[slot_number].level * 10;
        
        if (personnage->vie_actuelle > personnage->vie_max) {
            personnage->vie_actuelle = personnage->vie_max + personnage->skills[slot_number].level;
        }

        logMessage("Heal effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}

int skill_turtle(Personnage *personnage, int slot_number) {
    if (personnage->skills[slot_number].cooldown == 0) {
        personnage->skills[slot_number].lastUsed = 100;
        personnage->skills[slot_number].cooldown = 0;
        personnage->defense += personnage->skills[slot_number].level * 2;
        personnage->vitesse *= personnage->skills[slot_number].level * 0.05;
        logMessage("Turtle effectuée");
        return 1;
    } else {
        logMessage("Compétence en recharge");
        return 0;
    }
}


void skill_add(Personnage *personnage, int level, int skill_number) {
    for (int i = 0; i < 10; i++) {
        if (strcmp(personnage->skills[i].name, "") == 0) {
            strcpy(personnage->skills[i].name, config.skills.name[skill_number]);
            personnage->skills[i].level = config.skills.level[skill_number];
            personnage->skills[i].range = config.skills.range[skill_number];
            personnage->skills[i].maxExperience = config.skills.maxExperience[skill_number];
            personnage->skills[i].cooldown = config.skills.cooldown[skill_number];
            personnage->skills[i].lastUsed = config.skills.lastUsed[skill_number];
            personnage->skills[i].damage = config.skills.damage[skill_number];
            break;
        }
    }
}

void charger_skills(Personnage *personnage, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/skills.json", save);
    FILE *fichier = fopen(filepath, "r");

    if (fichier == NULL) {
        logMessage("Pas de skills");
        return;
    }

    char buffer[1024];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, fichier);
    fclose(fichier);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        logMessage("Erreur parsing fichier skills.json");
        return;
    }

    cJSON *skills_array = cJSON_GetObjectItem(json, "skills");
    if (skills_array) {
        for (int i = 0; i < cJSON_GetArraySize(skills_array); i++) {
            cJSON *skill_json = cJSON_GetArrayItem(skills_array, i);
            const char *name = cJSON_GetObjectItem(skill_json, "name")->valuestring;
            int level = cJSON_GetObjectItem(skill_json, "level")->valueint;
            int range = cJSON_GetObjectItem(skill_json, "range")->valueint;
            int maxExperience = cJSON_GetObjectItem(skill_json, "maxExperience")->valueint;
            int cooldown = cJSON_GetObjectItem(skill_json, "cooldown")->valueint;
            int lastUsed = cJSON_GetObjectItem(skill_json, "lastUsed")->valueint;
            int damage = cJSON_GetObjectItem(skill_json, "damage")->valueint;

            strcpy(personnage->skills[i].name, name);
            personnage->skills[i].level = level;
            personnage->skills[i].range = range;
            personnage->skills[i].maxExperience = maxExperience;
            personnage->skills[i].cooldown = cooldown;
            personnage->skills[i].lastUsed = lastUsed;
            personnage->skills[i].damage = damage;
        }
    }

    cJSON_Delete(json);
}

void enregistrer_skills(Personnage *personnage, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/skills.json", save);
    FILE *fichier = fopen(filepath, "w");

    if (fichier == NULL) {
        logMessage("Erreur ouverture fichier skills.json");
        return;
    }

    cJSON *json = cJSON_CreateObject();
    cJSON *skills_array = cJSON_CreateArray();

    for (int i = 0; i < 10; i++) {
        if (strcmp(personnage->skills[i].name, "") == 0) {
            break;
        }
        cJSON *skill = cJSON_CreateObject();
        cJSON_AddStringToObject(skill, "name", personnage->skills[i].name);
        cJSON_AddNumberToObject(skill, "level", personnage->skills[i].level);
        cJSON_AddNumberToObject(skill, "range", personnage->skills[i].range);
        cJSON_AddNumberToObject(skill, "maxExperience", personnage->skills[i].maxExperience);
        cJSON_AddNumberToObject(skill, "cooldown", personnage->skills[i].cooldown);
        cJSON_AddNumberToObject(skill, "lastUsed", personnage->skills[i].lastUsed);
        cJSON_AddNumberToObject(skill, "damage", personnage->skills[i].damage);
        cJSON_AddItemToArray(skills_array, skill);
    }

    cJSON_AddItemToObject(json, "skills", skills_array);

    char *json_string = cJSON_Print(json);
    fprintf(fichier, "%s\n", json_string);

    free(json_string);
    cJSON_Delete(json);
    fclose(fichier);
}

void cooldown_skills(Personnage *personnage) {
    for (int i = 0; i < 10; i++) {
        if (personnage->skills[i].cooldown > 0) {
            personnage->skills[i].cooldown--;
        }
    }
}