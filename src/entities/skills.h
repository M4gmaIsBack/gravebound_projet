#ifndef SKILLS_H
#define SKILLS_H

#include "character.h"
#include "../logs/logging.h"

int skill_escape(Personnage *personnage, int x, int y, int slot_number);
int skill_teleport(Personnage *personnage, int slot_number);
int skill_knockback(Personnage *personnage, int slot_number, int x, int y);
int skill_fire_ball(Personnage * personnage, int slot_number, int x, int y);
int skill_cage(Personnage *personnage, int slot_number, int x, int y);
int skill_light_speed(Personnage *personnage, int slot_number);
int skill_berzerk(Personnage *personnage, int slot_number);
int skill_kamikaze(Personnage *personnage, int slot_number, int x, int y);
int skill_heal(Personnage *personnage, int slot_number);
int skill_turtle(Personnage *personnage, int slot_number);
void skill_add(Personnage *personnage, char *nom, int level, int experience, int maxExperience, int cooldown, int lastUsed, int damage);
void charger_skills(Personnage *personnage, char *save);
void enregistrer_skills(Personnage *personnage, char *save);
void cooldown_skills(Personnage *personnage);

#endif