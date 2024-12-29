#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>
#include "../ui/graphique.h"
#include "../controller/controller.h"

#define JOYSTICK_ZONE_MORTE 4000
#define JOYSTICK_SENSIBILITE 0.01f
#define JOYSTICK_VITESSE_MAX 5.0f

int initManette();
void fermerManette();

void gererDeplacementCarte(SDL_Event *event, Jeu *jeu);
void gererDeplacementClavier(SDL_Event *event, Jeu *jeu);
void gererInputManette(Jeu *jeu, SDL_Event *event);
void toggleFullscreen(Jeu *jeu);

#endif
