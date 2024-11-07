#ifndef GAME_H
#define GAME_H

#include "../ui/graphique.h"

typedef struct {
    int running;
    Jeu jeu;
} Game;

int initJeu(Game *game);
void bouclePrincipale(Game *game);
void nettoyerRessources(Game *game);

#endif