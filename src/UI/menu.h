#ifndef MENU_H
#define MENU_H

#include "../game/game.h"
#include "../audio/audio.h"

typedef struct {
    SDL_Texture *background;
    SDL_Texture *buttons[4];
} MenuAssets;

void afficherMenuPrincipal(Game *game, AudioAssets *audio);

#endif
