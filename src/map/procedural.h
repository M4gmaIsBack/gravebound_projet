#ifndef PROCEDURAL_H
#define PROCEDURAL_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct{
    int x;
    int y;
    char *texture_path;
    SDL_Texture *texture;
    int region;
    float brightness_R;
    float brightness_G;
    float brightness_B;
} chunk;

typedef struct {
    int taille;
    chunk **cases;
    char **regions;
} carte;

carte creerCarte(int taille);
void detruireCarte(carte map);
carte genererCarte(carte map);
int enregistrerCarte(carte map);

#endif