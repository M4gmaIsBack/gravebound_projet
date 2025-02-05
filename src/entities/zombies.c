#include "zombies.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../game/game.h"
#include "../logs/logging.h"
#include "../entities/character.h"
#include <cjson/cJSON.h>
#include "../UI/cache.h"


Zombie** zombies = NULL;
int nombre_zombies = 0;
int capacite_zombies = 0;

Zombie* creer_zombie(float sante, float puissance_attaque, float vitesse, float vitesse_max, int x, int y, const char* type) {
    Zombie* nouveau_zombie = (Zombie*)malloc(sizeof(Zombie));
    if (nouveau_zombie == NULL) {
        fprintf(stderr, "Échec de l'allocation de mémoire pour le nouveau zombie\n");
        return NULL;
    }
    nouveau_zombie->sante = sante;
    nouveau_zombie->puissance_attaque = puissance_attaque;
    nouveau_zombie->vitesse = vitesse;
    nouveau_zombie->vitesse_max = vitesse_max;
    nouveau_zombie->x = x;
    nouveau_zombie->y = y;
    nouveau_zombie->type = strdup(type);
    if (strcmp(type, "boss") == 0) {
        nouveau_zombie->frameWidth = 128; 
        nouveau_zombie->frameHeight = 128;
    } else {
        nouveau_zombie->frameWidth = 32;
        nouveau_zombie->frameHeight = 32;
    }
    nouveau_zombie->currentFrame = 0;
    nouveau_zombie->totalFrames = 3;
    nouveau_zombie->direction = 0;
    nouveau_zombie->moving = 0;
    nouveau_zombie->texture = NULL;
    return nouveau_zombie;
}

int verifier_collision_zombies(int x, int y, int zombie_actuel) {
    for (int i = 0; i < nombre_zombies; i++) {
        if (i != zombie_actuel) {
            int dx = x - zombies[i]->x;
            int dy = y - zombies[i]->y;
            int distance = (int)sqrt(dx * dx + dy * dy);
            if (distance < ZOMBIE_DISTANCE_MIN) {
                return 1; // collision détectée =1
            }
        }
    }
    return 0; // 0 colision
}

void deplacer_vers_joueur(Zombie* zombie, int joueur_x, int joueur_y) {
    if (zombie == NULL) return;
    
    // distance entre le zombie et le joueur
    int dx = joueur_x - zombie->x;
    int dy = joueur_y - zombie->y;
    float distance = sqrt(dx * dx + dy * dy);

    // angle zombie -> joueur
    if (abs(dx) > abs(dy)) {
        zombie->direction = (dx > 0) ? 2 : 1; // droite : gauche
    } else {
        zombie->direction = (dy > 0) ? 0 : 3; // bas : haut
    }

    if (distance > 0) {
        // distance rafrachissement
        int nouveau_x = zombie->x + (int)(zombie->vitesse * dx / distance);
        int nouveau_y = zombie->y + (int)(zombie->vitesse * dy / distance);
        
        // vérifier collisions avec autres zombies
        int index_zombie = -1;
        for (int i = 0; i < nombre_zombies; i++) {
            if (zombies[i] == zombie) {
                index_zombie = i;
                break;
            }
        }
        
        // si pas de collision déplace zombie
        if (!verifier_collision_zombies(nouveau_x, nouveau_y, index_zombie)) {
            zombie->x = nouveau_x;
            zombie->y = nouveau_y;
            zombie->moving = 1;
            
           
            zombie->currentFrame = (zombie->currentFrame + 1) % zombie->totalFrames;
        } else {
            // si collision, zombie de angle différent
            float angle = atan2(dy, dx);
            float angle_alternative = angle + M_PI/2;  // perpendiculaire
            
            nouveau_x = zombie->x + (int)(zombie->vitesse * cos(angle_alternative));
            nouveau_y = zombie->y + (int)(zombie->vitesse * sin(angle_alternative));
            
            if (!verifier_collision_zombies(nouveau_x, nouveau_y, index_zombie)) {
                zombie->x = nouveau_x;
                zombie->y = nouveau_y;
            }
            
            zombie->moving = 1;
            zombie->currentFrame = (zombie->currentFrame + 1) % zombie->totalFrames;
        }
    } else {
        zombie->moving = 0;
        zombie->currentFrame = 1;
    }
    // printf("Zombie de type %s se déplace vers la position (%d, %d)\n", zombie->type, zombie->x, zombie->y);
}

void nettoyer_zombies() {
    for (int i = 0; i < nombre_zombies; i++) {
        if (zombies[i] && zombies[i]->sante <= 0) {
            free(zombies[i]->type);
            free(zombies[i]);
            zombies[i] = NULL;

            
            for (int j = i; j < nombre_zombies - 1; j++) {
                zombies[j] = zombies[j + 1];
            }
            zombies[--nombre_zombies] = NULL;
        }
    }
    // a changer si necessaire
    if (nombre_zombies < capacite_zombies / 2) {
        capacite_zombies /= 2;
        zombies = realloc(zombies, capacite_zombies * sizeof(Zombie*));
    }
}

void spawn_zombies(int centreX, int centreY, int rayon) {
    int nouveaux_zombies = rand() % config.zombies.spawn_rate[vague] + 1; // Entre 1 et 5 nouveaux zombies
    printf("Spawning %d zombies around (%d, %d) with radius %d\n", nouveaux_zombies, centreX, centreY, rayon);
    initialiser_zombies_autour_position(nouveaux_zombies, centreX, centreY, rayon);
}

void mettre_a_jour_zombies(int joueur_x, int joueur_y) {
    nettoyer_zombies(); // supprime les zombies morts
    for (int i = 0; i < nombre_zombies; i++) {
        deplacer_vers_joueur(zombies[i], joueur_x, joueur_y);

        if (zombies[i]->vitesse < zombies[i]->vitesse_max) {
            zombies[i]->vitesse += 0.005;
        }

        zombie_behaviour();
    }
}

void initialiser_zombies_autour_position(int nombre, int centreX, int centreY, int rayon) {
    if (nombre_zombies + nombre > capacite_zombies) {
        capacite_zombies = (nombre_zombies + nombre) * 2;
        zombies = realloc(zombies, capacite_zombies * sizeof(Zombie*));
    }

    for (int i = 0; i < nombre && nombre_zombies < config.zombies.max_zombies[vague]; i++) {
        int angleAleatoire = rand() % 360;
        int distanceAleatoire = rayon + (rand() % 100);

        // coordonnées polaires en cartésiennes
        double angleRad = angleAleatoire * M_PI / 180.0;
        int zombieX = centreX + (int)(cos(angleRad) * distanceAleatoire);
        int zombieY = centreY + (int)(sin(angleRad) * distanceAleatoire);

        const char *type[] = {"boss", "fast", "tank", "normal"};

        int rates[] = {5, 25, 45, 100};

        int typeAleatoire = rand() % 100;
        int typeZombie = 0;
        for (int j = 0; j < 4; j++) {
            if (typeAleatoire < rates[j]) {
                typeZombie = j;
                break;
            }
        }

        logMessage("Zombie %d %s apparaît en (%d, %d)", nombre_zombies, type[typeZombie], zombieX, zombieY);

        zombies[nombre_zombies++] = creer_zombie(config.zombies.max_health[vague], config.zombies.max_damage[vague], config.zombies.max_speed[vague], config.zombies.max_speed[vague], zombieX, zombieY, type[typeZombie]);
    }
}

void zombie_behaviour() {
    for (int i = 0; i < nombre_zombies; i++) {
        if (strcmp(zombies[i]->type, "fast") == 0) {
            zombies[i]->vitesse_max = config.zombies.max_speed[vague] * 1.7;

            if (zombies[i]->vitesse < zombies[i]->vitesse_max * 1.7) {
                zombies[i]->vitesse += 0.05;
            } else {
                zombies[i]->vitesse = zombies[i]->vitesse_max * 1.7;
            }

        } else if (strcmp(zombies[i]->type, "tank") == 0) {
            zombies[i]->sante += 0.00007 * config.zombies.max_health[vague];
            if (zombies[i]->sante > config.zombies.max_health[vague]) {
                zombies[i]->sante = config.zombies.max_health[vague];
            }

            zombies[i]->vitesse = zombies[i]->vitesse_max * 0.8;

        } else if (strcmp(zombies[i]->type, "boss") == 0) {
            zombies[i]->sante += 0.00007 * config.zombies.max_health[vague];
            if (zombies[i]->sante > (vague + 1) * config.zombies.max_health[vague]) {
                zombies[i]->sante =  (vague + 1) * config.zombies.max_health[vague];
            }

            if (zombies[i]->puissance_attaque < vague * config.zombies.max_damage[vague]) {
                zombies[i]->puissance_attaque += 0.0001 * config.zombies.max_damage[vague];
            }

            zombies[i]->vitesse_max = config.zombies.max_speed[vague] * 1.5;

            if (zombies[i]->vitesse < zombies[i]->vitesse_max * 1.3) {
                zombies[i]->vitesse += 0.05;
            } else {
                zombies[i]->vitesse = zombies[i]->vitesse_max * 1.3;
            }
        }
    }
}

void enregistrer_zombies(char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/zombies.json", save);
    FILE *fichier = fopen(filepath, "w");
    if (fichier == NULL) {
        logMessage("Erreur ouverture fichier zombies.json");
        return;
    }

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "nombre_zombies", nombre_zombies);

    cJSON *zombies_array = cJSON_CreateArray();
    for (int i = 0; i < nombre_zombies; i++) {
        cJSON *zombie = cJSON_CreateObject();
        cJSON_AddNumberToObject(zombie, "sante", zombies[i]->sante);
        cJSON_AddNumberToObject(zombie, "puissance_attaque", zombies[i]->puissance_attaque);
        cJSON_AddNumberToObject(zombie, "vitesse", zombies[i]->vitesse);
        cJSON_AddNumberToObject(zombie, "vitesse_max", zombies[i]->vitesse_max);
        cJSON_AddNumberToObject(zombie, "x", zombies[i]->x);
        cJSON_AddNumberToObject(zombie, "y", zombies[i]->y);
        cJSON_AddStringToObject(zombie, "type", zombies[i]->type);
        cJSON_AddItemToArray(zombies_array, zombie);
    }
    cJSON_AddItemToObject(json, "zombies", zombies_array);

    char *json_string = cJSON_Print(json);
    fprintf(fichier, "%s\n", json_string);

    free(json_string);
    cJSON_Delete(json);
    fclose(fichier);
}

void charger_zombies(char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/zombies.json", save);
    FILE *fichier = fopen(filepath, "r");
    if (fichier == NULL) {
        logMessage("Erreur ouverture fichier zombies.json");
        return;
    }

    char buffer[4096 * 10];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, fichier);
    fclose(fichier);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        logMessage("Erreur parsing fichier zombies.json");
        return;
    }

    cJSON *nombre_zombies_json = cJSON_GetObjectItem(json, "nombre_zombies");
    if (nombre_zombies_json) {
        nombre_zombies = nombre_zombies_json->valueint;
    }

    cJSON *zombies_array = cJSON_GetObjectItem(json, "zombies");
    if (zombies_array) {
        capacite_zombies = cJSON_GetArraySize(zombies_array);
        zombies = realloc(zombies, capacite_zombies * sizeof(Zombie*));
        for (int i = 0; i < capacite_zombies; i++) {
            cJSON *zombie_json = cJSON_GetArrayItem(zombies_array, i);
            int sante = cJSON_GetObjectItem(zombie_json, "sante")->valuedouble;
            int puissance_attaque = cJSON_GetObjectItem(zombie_json, "puissance_attaque")->valuedouble;
            float vitesse = cJSON_GetObjectItem(zombie_json, "vitesse")->valuedouble;
            float vitesse_max = cJSON_GetObjectItem(zombie_json, "vitesse_max")->valuedouble;
            int x = cJSON_GetObjectItem(zombie_json, "x")->valueint;
            int y = cJSON_GetObjectItem(zombie_json, "y")->valueint;
            const char *type = cJSON_GetObjectItem(zombie_json, "type")->valuestring;

            zombies[i] = creer_zombie(sante, puissance_attaque, vitesse, vitesse_max, x, y, type);
        }
        nombre_zombies = capacite_zombies;
    }

    cJSON_Delete(json);
}

void final_wave() {
    vague = 0;
}

void afficher_zombies(Jeu *jeu, int joueurCarteX, int joueurCarteY, int centreEcranX, int centreEcranY) {
    for (int i = 0; i < nombre_zombies; i++) {
        int zombieEcranX = zombies[i]->x - joueurCarteX + centreEcranX;
        int zombieEcranY = zombies[i]->y - joueurCarteY + centreEcranY;

        // collisions avec le personnage avec coordonnées carte
        int dx = zombies[i]->x - joueurCarteX;
        int dy = zombies[i]->y - joueurCarteY;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < 32) { // si le zombie est proche du personnage
            subirDegatsPersonnage(zombies[i]->puissance_attaque);
            // logMessage("Contact avec zombie! Distance: %f", distance);
        }

        if (zombies[i]->texture == NULL && strcmp(zombies[i]->type, "boss") == 1) {
            zombies[i]->texture = obtenirTexture(jeu->renderer, "./assets/zombies/zombies_spritesheet.png");        
        } else if (zombies[i]->texture == NULL && strcmp(zombies[i]->type, "boss") == 0) {
            zombies[i]->texture = obtenirTexture(jeu->renderer, "./assets/zombies/zombies_boss_spritesheet_.png");
        }

        if (zombies[i]->texture == NULL) {
            logMessage("Erreur chargement texture zombie");
            return;
        }
            
        SDL_Rect srcRect = {
            zombies[i]->currentFrame * zombies[i]->frameWidth,
            zombies[i]->direction * zombies[i]->frameHeight,
            zombies[i]->frameWidth,
            zombies[i]->frameHeight
        };

        SDL_Rect destRect = { 
            zombieEcranX, 
            zombieEcranY, 
            zombies[i]->frameWidth, 
            zombies[i]->frameHeight 
        };

        SDL_RenderCopy(jeu->renderer, zombies[i]->texture, &srcRect, &destRect);
        dessinerBarreDeVie(jeu->renderer, zombieEcranX, zombieEcranY - 10, 32, 5, zombies[i]->sante, 100);
    }
}