#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../game/game.h"

extern char* SAVE_FILE;

void dessinerPersonnage(SDL_Renderer* renderer, int x, int y);
void mettreAJourPersonnage(const Uint8* state);
void obtenirPositionPersonnage(int* x, int* y);
void fermerPersonnage();
void dessinerBarreDeVie(SDL_Renderer* renderer, int x, int y, int largeur, int hauteur, int sante, int santeMax);
void enregistrer_personnage(Game *game, char *save);
int charger_personnage(SDL_Renderer* renderer, Game *game, char *save);

int obtenirViePersonnage();
void subirDegatsPersonnage(int degats);
int estPersonnageVivant();
void attaquerZombies(); 
#define PORTEE_ATTAQUE 50 

#endif
