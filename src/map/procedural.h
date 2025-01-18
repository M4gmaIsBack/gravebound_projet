#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define LARGEUR_CASE 16
#define HAUTEUR_CASE 16

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
    SDL_Texture *texture;          // Texture modifiée
    SDL_Texture *originalTexture;  // Texture originale
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

typedef struct {
    unsigned int seed;
    float scale;
    float amplitude;
    int taille;
} GenerationParams;

GenerationParams initGenerationParams();

carte creerCarte(int taille);
carte genererCarte(carte map, GenerationParams params);
int sauvegarderParams(GenerationParams params, char *save);
GenerationParams chargerParams(char *save);

#endif
