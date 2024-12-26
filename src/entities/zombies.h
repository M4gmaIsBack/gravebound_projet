#ifndef ZOMBIES_H
#define ZOMBIES_H

#include <stdio.h>

typedef struct {
    int sante;
    int puissance_attaque;
    int vitesse;
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

Zombie* creer_zombie(int sante, int puissance_attaque, int vitesse, int x, int y, const char* type);
void attaquer(Zombie* zombie);
void subir_degats(Zombie* zombie, int degats);
void deplacer_vers_joueur(Zombie* zombie, int joueur_x, int joueur_y);
void journal_creation_zombie(Zombie* zombie);
void initialiser_zombies(int nombre);
void mettre_a_jour_zombies(int joueur_x, int joueur_y);
void initialiser_zombies_autour_position(int nombre, int centreX, int centreY, int rayon);

int verifier_collision_zombies(int x, int y, int zombie_actuel);
#define ZOMBIE_DISTANCE_MIN 32  // Distance minimale entre zombies

void detruire_zombie(int index);
int est_zombie_vivant(Zombie* zombie);
void nettoyer_zombies_morts();

#endif // ZOMBIES_H
