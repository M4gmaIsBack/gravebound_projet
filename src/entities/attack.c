#include "attack.h"
#include "zombies.h"
#include "../logs/logging.h"
#include <math.h>

Attack attacks[MAX_ATTACKS];
int num_attacks = 0;
static Uint32 last_auto_attack = 0;
SDL_Texture* attack_texture = NULL;

// Déclarations des fonctions utilitaires
int find_closest_zombie(int x, int y);
float calculate_distance(float x1, float y1, float x2, float y2);
float calculate_angle_to_target(float x1, float y1, float x2, float y2);

void init_attacks(SDL_Renderer* renderer) {
    for (int i = 0; i < MAX_ATTACKS; i++) {
        attacks[i].active = 0;
    }
    
    attack_texture = IMG_LoadTexture(renderer, "./assets/attacks/simple.png");
    if (!attack_texture) {
        logMessage("ERREUR: Impossible de charger simple.png: %s", IMG_GetError());
    }
}

void create_attack(float x, float y, float angle) {
    if (num_attacks >= MAX_ATTACKS) return;
    Attack* attack = &attacks[num_attacks++];
    attack->x = x;
    attack->y = y;
    attack->angle = angle;
    attack->speed = 8.0f;
    attack->frameWidth = 32;
    attack->frameHeight = 32;
    attack->currentFrame = 0;
    attack->totalFrames = 12;
    attack->lastUpdateTime = SDL_GetTicks();
    attack->active = 1;
    attack->target_zombie = find_closest_zombie(x, y);
}

void update_attacks(Jeu* jeu, int joueurX, int joueurY, int using_controller) {
    Uint32 current_time = SDL_GetTicks();
    if (using_controller && current_time - last_auto_attack >= AUTO_ATTACK_INTERVAL) {
        int target = find_closest_zombie(joueurX, joueurY);
        if (target != -1) {
            float angle = calculate_angle_to_target(joueurX, joueurY, zombies[target]->x, zombies[target]->y);
            create_attack(joueurX, joueurY, angle);
        }
        last_auto_attack = current_time;
    }

    for (int i = 0; i < num_attacks; i++) {
        Attack* attack = &attacks[i];
        if (!attack->active) continue;

        if (attack->target_zombie >= 0 && attack->target_zombie < nombre_zombies) {
            float targetX = zombies[attack->target_zombie]->x;
            float targetY = zombies[attack->target_zombie]->y;
            attack->angle = calculate_angle_to_target(attack->x, attack->y, targetX, targetY);
        }

        attack->x += cosf(attack->angle) * attack->speed;
        attack->y += sinf(attack->angle) * attack->speed;

        for (int j = 0; j < nombre_zombies; j++) {
            float dx = attack->x - zombies[j]->x;
            float dy = attack->y - zombies[j]->y;
            if (sqrtf(dx * dx + dy * dy) < 20) {
                zombies[j]->sante -= ATTACK_DAMAGE;
                if (zombies[j]->sante <= 0) zombies[j]->sante = 0;
                attack->active = 0;
                logMessage("Attaque touchée à x=%f, y=%f", attack->x, attack->y);
                break;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - attack->lastUpdateTime > 50) {  // Réduction du délai entre les frames (de 100 à 50ms)
            attack->currentFrame = (attack->currentFrame + 1) % attack->totalFrames;
            attack->lastUpdateTime = currentTime;
        }
    }

    int i = 0;
    while (i < num_attacks) {
        if (!attacks[i].active) {
            attacks[i] = attacks[--num_attacks];
        } else {
            i++;
        }
    }
}

void render_attacks(SDL_Renderer* renderer, Jeu* jeu, int joueurCarteX, int joueurCarteY) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    for (int i = 0; i < num_attacks; i++) {
        Attack* attack = &attacks[i];
        if (!attack->active || !attack_texture) continue;

        // Convertir les coordonnées monde en coordonnées écran
        int screenX = attack->x - joueurCarteX + jeu->largeurEcran / 2;
        int screenY = attack->y - joueurCarteY + jeu->hauteurEcran / 2;

        // Calcul des frames pour la spritesheet
        int frameX = (attack->currentFrame % 6) * 32;
        int frameY = (attack->currentFrame / 6) * 32;

        SDL_Rect srcRect = {frameX, frameY, 32, 32};
        SDL_Rect destRect = {screenX - 16, screenY - 16, 32, 32};

        SDL_RenderCopyEx(renderer, attack_texture, &srcRect, &destRect, 
                        attack->angle * 180/M_PI, NULL, SDL_FLIP_NONE);
    }
}

void cleanup_attacks(void) {
    num_attacks = 0;
}

int find_closest_zombie(int x, int y) {
    float closest_dist = 200;
    int closest_index = -1;
    for (int i = 0; i < nombre_zombies; i++) {
        float dist = calculate_distance(x, y, zombies[i]->x, zombies[i]->y);
        if (dist < closest_dist) {
            closest_dist = dist;
            closest_index = i;
        }
    }
    return closest_index;
}

float calculate_distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

float calculate_angle_to_target(float x1, float y1, float x2, float y2) {
    return atan2f(y2 - y1, x2 - x1);
}

void handle_mouse_attack(int mouseX, int mouseY, int playerX, int playerY) {
    static Uint32 last_attack_time = 0;
    Uint32 current_time = SDL_GetTicks();
    
    if (current_time - last_attack_time < ATTACK_COOLDOWN) return;
    
    float angle = atan2f(mouseY - playerY, mouseX - playerX);
    create_attack(playerX, playerY, angle);
    last_attack_time = current_time;
}
