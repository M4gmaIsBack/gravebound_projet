#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>

int chargerPersonnage(SDL_Renderer* renderer);
void dessinerPersonnage(SDL_Renderer* renderer, int carteX, int carteY);
void fermerPersonnage();

#endif
