#include "base.h"

Base init_base(Jeu *jeu) {
    Base base;
    base.x = (jeu->map.taille * LARGEUR_CASE) / 2;
    base.y = (jeu->map.taille * HAUTEUR_CASE) / 2;
    base.health = 100;
    base.max_health = 100;
    base.range = 100;
    base.damage = 10;
    base.attack_cooldown = 1000;
    base.last_attack_time = 0;
    return base;
}

void attack_base(Base* base, Zombie* zombie) {
    if (SDL_GetTicks() - base->last_attack_time > base->attack_cooldown) {
        base->last_attack_time = SDL_GetTicks();
        
        float distance;

        for (int i = 0; i < nombre_zombies; i++) {
            distance = sqrt(pow(zombie[i].x - base->x, 2) + pow(zombie[i].y - base->y, 2));
            if (distance < base->range) {
                zombie[i].sante -= base->damage;
                return;
            }
        }
    }
}

void knockback_base(Base* base, Zombie* zombie) {
    for (int i = 0; i < nombre_zombies; i++) {
        int dx = zombies[i]->x - base->x;
        int dy = zombies[i]->y - base->y;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < base->range) {
            float knockback_x = (dx / distance) * base->range;
            float knockback_y = (dy / distance) * base->range;
            zombies[i]->x += knockback_x;
            zombies[i]->y += knockback_y;
        }
    }
}

void heal_base(Base* base) {
    base->health += vague * 5;
    if (base->health > base->max_health) {
        base->health = base->max_health;
    }
}

void afficher_base(Jeu *jeu, SDL_Texture *baseTexture, Base *base) {
    int baseEcranX = base->x - jeu->carteX + jeu->largeurEcran / 2;
    int baseEcranY = base->y - jeu->carteY + jeu->hauteurEcran / 2;

    SDL_Rect destRect = { 
        baseEcranX - 32, // Adjust for base texture width
        baseEcranY - 32, // Adjust for base texture height
        64, // Base texture width
        64  // Base texture height
    };

    SDL_RenderCopy(jeu->renderer, baseTexture, NULL, &destRect);
}