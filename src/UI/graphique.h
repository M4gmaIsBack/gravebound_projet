#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../map/procedural.h"

<<<<<<< HEAD
=======
#define LARGEUR_ECRAN 800
#define HAUTEUR_ECRAN 600

#define LARGEUR_CASE 16
#define HAUTEUR_CASE 16

>>>>>>> 268c361a94de3d5f8b603f75e45dca57d2633fa5
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *carteTexture;
    int carteX, carteY;
    int largeurCarte, hauteurCarte;
<<<<<<< HEAD
    int largeurEcran, hauteurEcran;
=======
    carte map;
>>>>>>> 268c361a94de3d5f8b603f75e45dca57d2633fa5
} Jeu;

int initGraphique(Jeu *jeu);
int chargerCarte(Jeu *jeu);
void fermerGraphique(Jeu *jeu);
void majRendu(Jeu *jeu);
void toggleFullscreen(Jeu *jeu);

#endif
