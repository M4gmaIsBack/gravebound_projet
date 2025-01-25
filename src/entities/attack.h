#ifndef ATTACK_H
#define ATTACK_H

#include <SDL2/SDL.h>
#include "../game/game.h"

#define MAX_ATTACKS 20
#define ATTACK_DAMAGE 25
#define ATTACK_COOLDOWN 500  // en millisecondes
#define AUTO_ATTACK_INTERVAL 1000 // Intervalle entre les attaques auto (1 seconde)

typedef struct {
    float x, y, angle, speed;
    int frameWidth, frameHeight, currentFrame, totalFrames;
    Uint32 lastUpdateTime;
    int active, target_zombie;
} Attack;

extern Attack attacks[MAX_ATTACKS];
extern int num_attacks;

void init_attacks(SDL_Renderer* renderer);
void create_attack(float x, float y, float angle);
void update_attacks(Jeu* jeu, int joueurX, int joueurY, int using_controller);
void render_attacks(SDL_Renderer* renderer, Jeu* jeu, int joueurCarteX, int joueurCarteY);  // Modifier la déclaration de render_attacks pour inclure les coordonnées du joueur
void cleanup_attacks(void);
void handle_mouse_attack(int mouseX, int mouseY, int playerX, int playerY);

#endif // ATTACK_H
