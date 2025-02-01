#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../UI/cache.h"
#include "../UI/graphique.h"
#include "../map/procedural.h"

int chargerCarte(Jeu* jeu);
void chargerTextureChunk(chunk *c, SDL_Renderer *renderer);
void appliquerFiltreCouleur(SDL_Renderer *renderer, SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b);
float map_light_calculator(Jeu *jeu, int i, int j);
float character_light_calculator(Jeu *jeu, int x, int y);
void afficher_chunk(chunk *currentChunk, Jeu *jeu, int i, int j, float light);
void afficher_structure(Jeu *jeu, int i, int j, float light);
void environnement_effects(Jeu *jeu, Personnage *personnage);

#endif