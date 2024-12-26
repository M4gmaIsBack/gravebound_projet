#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>

int chargerPersonnage(SDL_Renderer* renderer);
void dessinerPersonnage(SDL_Renderer* renderer, int x, int y);
void mettreAJourPersonnage(const Uint8* state);
void obtenirPositionPersonnage(int* x, int* y);
void fermerPersonnage();

// Nouvelles fonctions pour le système de combat
int obtenirViePersonnage();
void subirDegatsPersonnage(int degats);
int estPersonnageVivant();
void attaquerZombies(); // Pour gérer l'attaque du personnage
#define PORTEE_ATTAQUE 50 // Distance d'attaque du personnage

#endif
