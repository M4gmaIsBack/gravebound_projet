#ifndef MENU_PERSONNAGE_H
#define MENU_PERSONNAGE_H

#include <SDL2/SDL.h>
#include "../game/game.h"  // Correction du chemin (suppression de /jeu)
#include "../audio/audio.h"  // Ajout pour AudioAssets

typedef struct {
    SDL_Texture *background;
    SDL_Texture *persos[3];  // Modification de 2 à 3 personnages
    SDL_Texture *flecheGauche;
    SDL_Texture *flecheDroite;
} MenuPersoAssets;

extern const char* cheminSpriteSheetPersonnage;  // Déclaration de la variable globale

void afficherMenuPersonnage(Game *game, AudioAssets *audio);

#endif
