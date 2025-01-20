#include "skills.h"
#include "character.h"
#include "zombies.h"

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
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/skills.txt", save);
    FILE *fichier = fopen(filepath, "r");

    if (fichier == NULL) {
        logMessage("Pas de skills");
    } else {
        for (int i = 0; i < 10; i++) {
            printf("lecture skill %d\n", i);
            if (fscanf(fichier, "%s %d %d %d %d %d %d\n", personnage->skills[i].name, &personnage->skills[i].level, &personnage->skills[i].range, &personnage->skills[i].maxExperience, &personnage->skills[i].cooldown, &personnage->skills[i].lastUsed, &personnage->skills[i].damage) != 7) {
                logMessage("fin de lecture, %d skills chargées", i);
                break;
            }
        }
        fclose(fichier);
    }
}

void enregistrer_skills(Personnage *personnage, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/skills.txt", save);
    FILE *fichier = fopen(filepath, "w");

    if (fichier == NULL) {
        logMessage("Erreur ouverture fichier skills.txt");
        return;
    }

    for (int i = 0; i < 10; i++) {
        if (strcmp(personnage->skills[i].name, "") == 0) {
            break;
        }
        printf("%s %d %d %d %d %d %d\n", personnage->skills[i].name, personnage->skills[i].level, personnage->skills[i].range, personnage->skills[i].maxExperience, personnage->skills[i].cooldown, personnage->skills[i].lastUsed, personnage->skills[i].damage);
        fprintf(fichier, "%s %d %d %d %d %d %d\n", personnage->skills[i].name, personnage->skills[i].level, personnage->skills[i].range, personnage->skills[i].maxExperience, personnage->skills[i].cooldown, personnage->skills[i].lastUsed, personnage->skills[i].damage);
    }
    fclose(fichier);
}

void cooldown_skills(Personnage *personnage) {
    for (int i = 0; i < 10; i++) {
        if (personnage->skills[i].cooldown > 0) {
            personnage->skills[i].cooldown--;
        }
    }
}