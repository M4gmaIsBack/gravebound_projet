#include "zombies.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../game/game.h"
#include "../logs/logging.h"
#include "../entities/character.h"


Zombie* zombies[MAX_ZOMBIES];
int nombre_zombies = 0;

Zombie* creer_zombie(int sante, int puissance_attaque, int vitesse, int x, int y, const char* type) {
    Zombie* nouveau_zombie = (Zombie*)malloc(sizeof(Zombie));
    if (nouveau_zombie == NULL) {
        fprintf(stderr, "Échec de l'allocation de mémoire pour le nouveau zombie\n");
        return NULL;
    }
    nouveau_zombie->sante = sante;
    nouveau_zombie->puissance_attaque = puissance_attaque;
    nouveau_zombie->vitesse = vitesse;
    nouveau_zombie->x = x;
    nouveau_zombie->y = y;
    nouveau_zombie->type = strdup(type);
    nouveau_zombie->frameWidth = 32;
    nouveau_zombie->frameHeight = 32;
    nouveau_zombie->currentFrame = 0;
    nouveau_zombie->totalFrames = 3;
    nouveau_zombie->direction = 0;
    nouveau_zombie->moving = 0;
    journal_creation_zombie(nouveau_zombie);
    return nouveau_zombie;
}

void attaquer(Zombie* zombie) {
    if (zombie == NULL) return;
    printf("Zombie de type %s attaque avec une puissance de %d\n", zombie->type, zombie->puissance_attaque);
}

void subir_degats(Zombie* zombie, int degats) {
    if (zombie == NULL) return;
    zombie->sante -= degats;
    printf("Zombie de type %s subit %d dégâts, santé restante: %d\n", zombie->type, degats, zombie->sante);
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

void journal_creation_zombie(Zombie* zombie) {
    if (zombie == NULL) return;
    printf("Création d'un zombie de type %s avec santé: %d, puissance d'attaque: %d, vitesse: %d à la position (%d, %d)\n", 
           zombie->type, zombie->sante, zombie->puissance_attaque, zombie->vitesse, zombie->x, zombie->y);
}

void initialiser_zombies(int nombre) {
    for (int i = 0; i < nombre && i < MAX_ZOMBIES; i++) {
        zombies[i] = creer_zombie(100, 10, 2, rand() % 800, rand() % 600, "normal");
        nombre_zombies++;
    }
}

void initialiser_zombies_autour_position(int nombre, int centreX, int centreY, int rayon) {
    for (int i = 0; i < nombre && i < MAX_ZOMBIES; i++) {
        int angleAleatoire = rand() % 360;
        int distanceAleatoire = rand() % rayon;
        
        // Convertir coordonnées polaires en cartésiennes
        double angleRad = angleAleatoire * M_PI / 180.0;
        int zombieX = centreX + (int)(cos(angleRad) * distanceAleatoire);
        int zombieY = centreY + (int)(sin(angleRad) * distanceAleatoire);
        
        zombies[i] = creer_zombie(100, 10, 2, zombieX, zombieY, "normal");
        nombre_zombies++;
    }
}

void mettre_a_jour_zombies(int joueur_x, int joueur_y) {
    for (int i = 0; i < nombre_zombies; i++) {
        deplacer_vers_joueur(zombies[i], joueur_x, joueur_y);
    }
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
            logMessage("Contact avec zombie! Distance: %f", distance);
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