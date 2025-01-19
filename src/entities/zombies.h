#ifndef ZOMBIES_H
#define ZOMBIES_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../game/game.h"

#define MAX_ZOMBIES 100
#define ZOMBIE_DISTANCE_MIN 10

typedef struct {
    int sante;
    int puissance_attaque;
    float vitesse;
    float vitesse_max;
    int x;
    int y;
    char* type;
    int frameWidth;            // Largeur d'une frame
    int frameHeight;           // Hauteur d'une frame
    int currentFrame;          // Frame actuelle
    int totalFrames;           // Nombre total de frames
    int direction;             // Direction (0 = bas, 1 = gauche, 2 = droite, 3 = haut)
    int moving;                // En mouvement ou non
} Zombie;

extern Zombie* zombies[];
extern int nombre_zombies;

void deplacer_vers_joueur(Zombie* zombie, int joueur_x, int joueur_y);
void mettre_a_jour_zombies(int joueur_x, int joueur_y);
void initialiser_zombies_autour_position(int nombre, int centreX, int centreY, int rayon);
void afficher_zombies(Jeu *jeu, SDL_Texture *zombieTexture, int joueurCarteX, int joueurCarteY, int centreEcranX, int centreEcranY);
int verifier_collision_zombies(int x, int y, int zombie_actuel);
void nettoyer_zombies();
void spawn_zombies(int centreX, int centreY, int rayon);
Zombie *creer_zombie(int sante, int puissance_attaque, float vitesse, float vitesse_max, int x, int y, const char *type);
void enregistrer_zombies(char *save);
void charger_zombies(char *save);

#endif // ZOMBIES_H
