#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <SDL2/SDL.h>

typedef struct Structure {
    char* texture_path;
    SDL_Texture* texture;
    int largeur;
    int hauteur;
} Structure;

int charger_structure(SDL_Renderer* renderer, Structure* structure);
void dessiner_structure(SDL_Renderer* renderer, Structure* structure, int x, int y);
void liberer_structure(Structure* structure);