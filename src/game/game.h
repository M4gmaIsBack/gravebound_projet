#ifndef GAME_H
#define GAME_H

#include "../map/procedural.h"
#include "../entities/character.h"
#include "../entities/skills.h"
#include "../game/config.h"
#include "time.h"



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

typedef struct {
    int running;
    Jeu jeu;
} Game;

extern Config config;

int initJeu(Game *game);
void bouclePrincipale(Game *game, char *save, Personnage *personnage);
void nettoyerRessources(Game *game);
void toggleFullscreen(Jeu *jeu);
void lancerJeu(Game *game, char *save);
void enregistrer_progression(Game *game, char *save, Personnage *personnage);
Personnage charger_progression(Game *game, char *save);
void charger_coordonnees(Jeu *jeu, char *save);
void enregistrer_coordonnees(Jeu *jeu, char *save);

#endif
