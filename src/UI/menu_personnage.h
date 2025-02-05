#ifndef MENU_PERSONNAGE_H
#define MENU_PERSONNAGE_H

#include <SDL2/SDL.h>
#include "../game/game.h"
#include "../audio/audio.h"

typedef struct {
    SDL_Texture *background;
    SDL_Texture *persos[3]; 
    SDL_Texture *flecheGauche;
    SDL_Texture *flecheDroite;
} MenuPersoAssets;

extern const char* cheminSpriteSheetPersonnage; 

void afficherMenuPersonnage(Game *game, AudioAssets *audio);
static void gererClavierMenuPersonnage(SDL_Event *e, int *selection, int *quitter, AudioAssets *audio);
static void renderMenuPersonnage(Game *game, MenuPersoAssets *assets, int selection);
#endif
