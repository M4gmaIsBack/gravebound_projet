#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    char name[50];
    int level;
    int range;
    int maxExperience;
    int cooldown;
    int lastUsed;
    int damage;
} Skill;


typedef struct {
    SDL_Texture* texture;      // Texture du personnage
    int frameWidth;            // Largeur d'une frame
    int frameHeight;           // Hauteur d'une frame
    int currentFrame;          // Frame actuelle
    int totalFrames;           // Nombre de frames par animation
    int direction;             // Direction actuelle (0 = bas, 1 = gauche, 2 = droite, 3 = haut)
    int moving;                // Indique si le personnage est en mouvement
    int x;                     // Position x du personnage
    int y;                     // Position y du personnage
    int vie_max;
    int vie_actuelle;
    int force_attaque;
    int invincibilite;         // Compteur pour l'invincibilité temporaire après avoir subi des dégâts
    float vitesse;               // Vitesse de déplacement du personnage
    float vitesse_max;
    float defense;
    Skill skills[10];
} Personnage;

void dessinerPersonnage(SDL_Renderer* renderer, int x, int y);
void mettreAJourPersonnage(const Uint8* state);
void obtenirPositionPersonnage(int* x, int* y);
void fermerPersonnage();
void dessinerBarreDeVie(SDL_Renderer* renderer, int x, int y, int largeur, int hauteur, int sante, int santeMax);
void enregistrer_personnage(char *save);
Personnage charger_personnage(SDL_Renderer* renderer, char *save);

int obtenirViePersonnage();
void subirDegatsPersonnage(int degats);
int estPersonnageVivant();
void attaquerZombies(); 
#define PORTEE_ATTAQUE 50 

#endif
