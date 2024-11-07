#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define LARGEUR_ECRAN 800
#define HAUTEUR_ECRAN 600

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *carteTexture;
    int carteX, carteY;
    int largeurCarte, hauteurCarte;
} Jeu;

int initGraphique(Jeu *jeu);
void fermerGraphique(Jeu *jeu);
void majRendu(Jeu *jeu);
int chargerCarte(Jeu *jeu);

#endif