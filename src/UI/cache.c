#include "cache.h"
#include "../logs/logging.h"

void libererCache() {
    TextureCache* courant = cache;
    while (courant) {
        TextureCache* suivant = courant->suivant;
        SDL_DestroyTexture(courant->texture);
        free(courant->chemin);
        free(courant);
        courant = suivant;
    }
    cache = NULL;
}

SDL_Texture* obtenirTexture(SDL_Renderer* renderer, const char* chemin) {
    TextureCache* courant = cache;
    while (courant) {
        if (strcmp(courant->chemin, chemin) == 0) {
            return courant->texture;
        }
        courant = courant->suivant;
    }

    // Charge une nouvelle texture si elle n'est pas dans le cache
    SDL_Surface* surface = IMG_Load(chemin);
    if (!surface) {
        logMessage("Erreur chargement image: %s", IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        logMessage("Erreur création texture: %s", SDL_GetError());
        return NULL;
    }

    // Ajouter au cache
    TextureCache* nouvelElement = malloc(sizeof(TextureCache));
    nouvelElement->chemin = strdup(chemin);
    nouvelElement->texture = texture;
    nouvelElement->suivant = cache;
    cache = nouvelElement;

    return texture;
}