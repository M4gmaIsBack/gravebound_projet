#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../map/procedural.h"
#include "../game/time.h"


#define LARGEUR_CASE 16
#define HAUTEUR_CASE 16
#define FPS 60
#define FRAME_DELAY (1000 / FPS)

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

typedef struct {
    SDL_Window *window; 
    SDL_Renderer *renderer;
    SDL_Texture *carteTexture;
    int carteX, carteY;
    int startX, startY, endX, endY;
    int largeurCarte, hauteurCarte;
    int largeurEcran, hauteurEcran;
    carte map;
    time countdown;
} Jeu;

int initGraphique(Jeu *jeu);
void fermerGraphique(Jeu *jeu);
int chargerCarte(Jeu *jeu);
void chargerTextureChunk(chunk *c, SDL_Renderer *renderer);
void majRendu(Jeu *jeu);

#endif
