#include "structure.h"
#include "../logs/logging.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int charger_structure(SDL_Renderer* renderer, Structure* structure) {
    SDL_Surface* surface = IMG_Load(structure.texture_path);
    if (!surface) {
        logMessage("Erreur de chargement de la sprite-sheet des structures: %s", IMG_GetError());
        return 0;
    }

    structure.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!structure.texture) {
        logMessage("Erreur de création de la texture des structures: %s", SDL_GetError());
        return 0;
    }


    return 1;
}

void dessiner_structure(SDL_Renderer* renderer, Structure* structure, int x, int y) {
    SDL_Rect dest = {
        x,
        y,
        structure.largeur,
        structure.hauteur
    };

    SDL_RenderCopy(renderer, structure.texture, NULL, &dest);
}

void liberer_structure(Structure* structure) {
    SDL_DestroyTexture(structure.texture);
    free(structure.texture_path);
}