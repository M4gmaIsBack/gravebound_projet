#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "cache.h"
#include "graphique.h"
#include "../map/procedural.h"

int chargerCarte(Jeu* jeu);
void chargerTextureChunk(chunk *c, SDL_Renderer *renderer);
void appliquerFiltreCouleur(SDL_Renderer *renderer, SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b);
float light_calculator(Jeu *jeu, int i, int j);
void afficher_chunk(chunk *currentChunk, Jeu *jeu, int i, int j, float light);
void afficher_structure(Jeu *jeu, int i, int j, float light);

#endif