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

Zombie* zombies[MAX_ZOMBIES];
int nombre_zombies = 0;

Zombie* creer_zombie(int sante, int puissance_attaque, float vitesse, float vitesse_max, int x, int y, const char* type) {
    Zombie* nouveau_zombie = (Zombie*)malloc(sizeof(Zombie));
    if (nouveau_zombie == NULL) {
        fprintf(stderr, "Échec de l'allocation de mémoire pour le nouveau zombie\n");
        return NULL;
    }
    nouveau_zombie->sante = sante;
    nouveau_zombie->puissance_attaque = puissance_attaque;
    nouveau_zombie->vitesse = vitesse;
    nouveau_zombie->vitesse_max = vitesse;
    nouveau_zombie->x = x;
    nouveau_zombie->y = y;
    nouveau_zombie->type = strdup(type);
    nouveau_zombie->frameWidth = 32;
    nouveau_zombie->frameHeight = 32;
    nouveau_zombie->currentFrame = 0;
    nouveau_zombie->totalFrames = 3;
    nouveau_zombie->direction = 0;
    nouveau_zombie->moving = 0;
    return nouveau_zombie;
}

int verifier_collision_zombies(int x, int y, int zombie_actuel) {
    for (int i = 0; i < nombre_zombies; i++) {
        if (i != zombie_actuel) {
            int dx = x - zombies[i]->x;
            int dy = y - zombies[i]->y;
            int distance = (int)sqrt(dx * dx + dy * dy);
            if (distance < ZOMBIE_DISTANCE_MIN) {
                return 1; // Collision détectée
            }
        }
    }
    return 0; // Pas de collision
}

void deplacer_vers_joueur(Zombie* zombie, int joueur_x, int joueur_y) {
    if (zombie == NULL) return;
    
    // Calculer la direction souhaitée
    int dx = joueur_x - zombie->x;
    int dy = joueur_y - zombie->y;
    float distance = sqrt(dx * dx + dy * dy);

    // Mettre à jour la direction d'animation
    if (abs(dx) > abs(dy)) {
        zombie->direction = (dx > 0) ? 2 : 1; // droite : gauche
    } else {
        zombie->direction = (dy > 0) ? 0 : 3; // bas : haut
    }

    if (distance > 0) {
        // Calculer la nouvelle position potentielle
        int nouveau_x = zombie->x + (int)(zombie->vitesse * dx / distance);
        int nouveau_y = zombie->y + (int)(zombie->vitesse * dy / distance);
        
        // Vérifier les collisions pour cette position
        int index_zombie = -1;
        for (int i = 0; i < nombre_zombies; i++) {
            if (zombies[i] == zombie) {
                index_zombie = i;
                break;
            }
        }
        
        // Si pas de collision, déplacer le zombie
        if (!verifier_collision_zombies(nouveau_x, nouveau_y, index_zombie)) {
            zombie->x = nouveau_x;
            zombie->y = nouveau_y;
            zombie->moving = 1;
            
            // Animation
            zombie->currentFrame = (zombie->currentFrame + 1) % zombie->totalFrames;
        } else {
            // En cas de collision, essayer de se déplacer latéralement
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

            // Décaler les autres zombies pour combler le vide
            for (int j = i; j < nombre_zombies - 1; j++) {
                zombies[j] = zombies[j + 1];
            }
            zombies[--nombre_zombies] = NULL;
        }
    }
}

void spawn_zombies(int centreX, int centreY, int rayon) {
    int nouveaux_zombies = rand() % config.zombies.spawn_rate[vague] + 1; // Entre 1 et 5 nouveaux zombies
    printf("Spawning %d zombies around (%d, %d) with radius %d\n", nouveaux_zombies, centreX, centreY, rayon);
    initialiser_zombies_autour_position(nouveaux_zombies, centreX, centreY, rayon);
}

void mettre_a_jour_zombies(int joueur_x, int joueur_y) {
    nettoyer_zombies(); // Supprime les zombies morts
    for (int i = 0; i < nombre_zombies; i++) {
        deplacer_vers_joueur(zombies[i], joueur_x, joueur_y);

        if (zombies[i]->vitesse < zombies[i]->vitesse_max) {
            zombies[i]->vitesse += 0.005;
        }
    }
}

void initialiser_zombies_autour_position(int nombre, int centreX, int centreY, int rayon) {
    for (int i = 0; i < nombre && nombre_zombies < MAX_ZOMBIES; i++) {
        int angleAleatoire = rand() % 360;
        int distanceAleatoire = rayon + (rand() % 100);

        // Convertir coordonnées polaires en cartésiennes
        double angleRad = angleAleatoire * M_PI / 180.0;
        int zombieX = centreX + (int)(cos(angleRad) * distanceAleatoire);
        int zombieY = centreY + (int)(sin(angleRad) * distanceAleatoire);

        logMessage("Zombie %d apparaît en (%d, %d)", nombre_zombies, zombieX, zombieY);

        zombies[nombre_zombies++] = creer_zombie(config.zombies.max_health[vague], config.zombies.max_damage[vague], config.zombies.max_speed[vague], config.zombies.max_speed[vague], zombieX, zombieY, "normal");
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

    char buffer[4096];
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
        for (int i = 0; i < cJSON_GetArraySize(zombies_array); i++) {
            cJSON *zombie_json = cJSON_GetArrayItem(zombies_array, i);
            int sante = cJSON_GetObjectItem(zombie_json, "sante")->valueint;
            int puissance_attaque = cJSON_GetObjectItem(zombie_json, "puissance_attaque")->valueint;
            float vitesse = cJSON_GetObjectItem(zombie_json, "vitesse")->valuedouble;
            float vitesse_max = cJSON_GetObjectItem(zombie_json, "vitesse_max")->valuedouble;
            int x = cJSON_GetObjectItem(zombie_json, "x")->valueint;
            int y = cJSON_GetObjectItem(zombie_json, "y")->valueint;
            const char *type = cJSON_GetObjectItem(zombie_json, "type")->valuestring;

            zombies[i] = creer_zombie(sante, puissance_attaque, vitesse, vitesse_max, x, y, type);
        }
    }

    cJSON_Delete(json);
}

void afficher_zombies(Jeu *jeu, SDL_Texture *zombieTexture, int joueurCarteX, int joueurCarteY, int centreEcranX, int centreEcranY) {
    for (int i = 0; i < nombre_zombies; i++) {
        int zombieEcranX = zombies[i]->x - joueurCarteX + centreEcranX;
        int zombieEcranY = zombies[i]->y - joueurCarteY + centreEcranY;

        // Vérifier les collisions avec le personnage en utilisant les coordonnées de la carte
        int dx = zombies[i]->x - joueurCarteX;
        int dy = zombies[i]->y - joueurCarteY;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < 32) { // Si le zombie est proche du personnage
            subirDegatsPersonnage(zombies[i]->puissance_attaque);
            // logMessage("Contact avec zombie! Distance: %f", distance);
        }

        // Rendu du zombie
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

        SDL_RenderCopy(jeu->renderer, zombieTexture, &srcRect, &destRect);
        dessinerBarreDeVie(jeu->renderer, zombieEcranX, zombieEcranY - 10, 32, 5, zombies[i]->sante, 100);
    }
}