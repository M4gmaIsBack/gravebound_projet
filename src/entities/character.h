#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>

// Charge la sprite-sheet du personnage
int chargerPersonnage(SDL_Renderer* renderer);

// Dessine le personnage à l'écran
void dessinerPersonnage(SDL_Renderer* renderer, int x, int y);

// Met à jour l'animation en fonction des entrées
void mettreAJourPersonnage(const Uint8* state);

// Libère les ressources liées au personnage
void fermerPersonnage();

#endif
