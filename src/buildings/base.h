#ifndef BASE_H
#define BASE_H

#include <SDL2/SDL.h>
#include "../entities/zombies.h"
#include "../game/config.h"
#include "../game/game.h"

Base init_base(Jeu *jeu);
void attack_base(Base* base, Zombie* zombie);
void knockback_base(Base* base, Zombie* zombie);
void heal_base(Base* base);
void afficher_base(Jeu *jeu, SDL_Texture *baseTexture, Base *base);

#endif