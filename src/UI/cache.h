#ifndef CACHE_H
#define CACHE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct TextureCache {
    char* chemin;
    SDL_Texture* texture;
    struct TextureCache* suivant;
} TextureCache;

static TextureCache* cache = NULL;
void libererCache();
SDL_Texture* obtenirTexture(SDL_Renderer* renderer, const char* chemin);

#endif