#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>
#include "../ui/graphique.h"
#include "../controller/controller.h"
#include "../entities/character.h"
#include "../entities/skills.h"
#include "../game/config.h"

#define JOYSTICK_ZONE_MORTE 4000
#define JOYSTICK_SENSIBILITE 0.01f

int initManette();
void fermerManette();

void gererDeplacementCarte(SDL_Event *event, Jeu *jeu, Personnage *personnage);
void gererDeplacementClavier(SDL_Event *event, Jeu *jeu, Personnage *personnage);
void gererInputManette(Jeu *jeu, SDL_Event *event);
void skill_activation(SDL_Event *event, Game *game, Personnage *personnage, int slot_selected);
int skill_selection(SDL_Event *event, Game *game, Personnage *personnage, int slot_selected);
void toggleFullscreen(Jeu *jeu);

#endif
