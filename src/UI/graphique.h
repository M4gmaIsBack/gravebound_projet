#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../map/procedural.h"

#define LARGEUR_CASE 16
#define HAUTEUR_CASE 16

typedef struct {
    SDL_Window *window; 
    SDL_Renderer *renderer;
    SDL_Texture *carteTexture;
    int carteX, carteY;
    int startX, startY, endX, endY;
    int largeurCarte, hauteurCarte;
    int largeurEcran, hauteurEcran;
    carte map;                   
} Jeu;

int initGraphique(Jeu *jeu);
void fermerGraphique(Jeu *jeu);
int chargerCarte(Jeu *jeu);
void chargerTextureChunk(chunk *c, SDL_Renderer *renderer);
void majRendu(Jeu *jeu);
void toggleFullscreen(Jeu *jeu);
void gererInputManette(Jeu *jeu, SDL_Event *event);
void boucleJeu(Jeu *jeu);

#endif
