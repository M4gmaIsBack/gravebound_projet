#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>
#include "../ui/graphique.h"

// On definie la sensibilité de la manette
#define JOYSTICK_ZONE_MORTE 4000
#define JOYSTICK_SENSIBILITE 0.01f
#define JOYSTICK_VITESSE_MAX 5.0f


int initManette();

void fermerManette();

// Gere le deplacement de la carte en fonction de la manette et clavier souris
// Prend en parametre SDL et la structure de jeu
void gererDeplacementCarte(SDL_Event *event, Jeu *jeu);
void gererDeplacementClavier(SDL_Event *event, Jeu *jeu);


#endif