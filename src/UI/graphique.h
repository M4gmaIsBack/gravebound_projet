#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../map/procedural.h"

typedef struct {
    SDL_Window *window; 
    SDL_Renderer *renderer;
    SDL_Texture *carteTexture;
    int carteX, carteY;
    int largeurCarte, hauteurCarte;
    int largeurEcran, hauteurEcran;
    carte map;                   
} Jeu;

int initGraphique(Jeu *jeu);
int chargerCarte(Jeu *jeu);
void fermerGraphique(Jeu *jeu);
void majRendu(Jeu *jeu);
void toggleFullscreen(Jeu *jeu);
void gererInputManette(Jeu *jeu, SDL_Event *event);

#endif
