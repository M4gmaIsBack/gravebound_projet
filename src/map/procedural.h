#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../game/config.h"

#define LARGEUR_CASE 16
#define HAUTEUR_CASE 16

extern Config config;

typedef struct {
    char *texture_path;
    SDL_Texture *texture;
    int height;
    int width;
} structure;

typedef struct {
    int x;
    int y;
    char *texture_path;
    structure structure;
    SDL_Texture *texture;         
    SDL_Texture *originalTexture;  
    int region;
    float brightness_R;
    float brightness_G;
    float brightness_B;
    int loaded;
} chunk;

typedef struct {
    int taille;
    chunk **cases;
    char **regions;
} carte;



GenerationParams initGenerationParams();

carte creerCarte(int taille);
carte genererCarte(carte map, GenerationParams params);
int sauvegarderParams(GenerationParams params, char *save);
GenerationParams chargerParams(char *save);

#endif
