#include "character.h"
#include "../logs/logging.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "zombies.h"

// Structure pour le personnage
typedef struct {
    SDL_Texture* texture;      // Texture du personnage
    int frameWidth;            // Largeur d'une frame
    int frameHeight;           // Hauteur d'une frame
    int currentFrame;          // Frame actuelle
    int totalFrames;           // Nombre de frames par animation
    int direction;             // Direction actuelle (0 = bas, 1 = gauche, 2 = droite, 3 = haut)
    int moving;                // Indique si le personnage est en mouvement
    int x;                     // Position x du personnage
    int y;                     // Position y du personnage
    int vie_max;
    int vie_actuelle;
    int force_attaque;
    int invincibilite;         // Compteur pour l'invincibilité temporaire après avoir subi des d��gâts
    int vitesse;               // Vitesse de déplacement du personnage
} Personnage;

Personnage personnage;

int chargerPersonnage(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("./assets/character/spritesheet.png");
    if (!surface) {
        logMessage("Erreur de chargement de la sprite-sheet du personnage: %s", IMG_GetError());
        return 0;
    }

    personnage.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!personnage.texture) {
        logMessage("Erreur de création de la texture du personnage: %s", SDL_GetError());
        return 0;
    }

    personnage.frameWidth = 32;
    personnage.frameHeight = 32;
    personnage.currentFrame = 0;
    personnage.totalFrames = 3;
    personnage.direction = 0;
    personnage.moving = 0;
    personnage.x = 0; // Initialiser la position x
    personnage.y = 0; // Initialiser la position y
    personnage.vie_max = 100;
    personnage.vie_actuelle = 100;
    personnage.force_attaque = 25;
    personnage.invincibilite = 0;
    personnage.vitesse = 2; // Initialiser la vitesse de déplacement

    logMessage("Sprite-sheet du personnage chargée.");
    return 1;
}

void mettreAJourPersonnage(const Uint8* state) {
    personnage.moving = 0;

    // Détermine la direction selon les touches pressées (ZQSD)
    if (state[SDL_SCANCODE_W]) {
        personnage.direction = 3;
        personnage.y -= personnage.vitesse;
        personnage.moving = 1;
    } else if (state[SDL_SCANCODE_S]) {
        personnage.direction = 0;
        personnage.y += personnage.vitesse;
        personnage.moving = 1;
    } else if (state[SDL_SCANCODE_A]) {
        personnage.direction = 1;
        personnage.x -= personnage.vitesse;
        personnage.moving = 1;
    } else if (state[SDL_SCANCODE_D]) { 
        personnage.direction = 2;
        personnage.x += personnage.vitesse;
        personnage.moving = 1;
    }

    // Met à jour la frame uniquement si le personnage bouge
    if (personnage.moving) {
        personnage.currentFrame = (personnage.currentFrame + 1) % personnage.totalFrames;
    } else {
        personnage.currentFrame = 1;
    }
    
    // Mise à jour du compteur d'invincibilité
    if (personnage.invincibilite > 0) {
        personnage.invincibilite--;
    }

    // Attaque avec la touche ESPACE
    if (state[SDL_SCANCODE_SPACE]) {
        attaquerZombies();
    }
}

void dessinerPersonnage(SDL_Renderer* renderer, int x, int y) {
    if (!personnage.texture) {
        logMessage("Erreur: la texture du personnage n'est pas chargée.");
        return;
    }

    SDL_Rect srcRect = {
        personnage.currentFrame * personnage.frameWidth,
        personnage.direction * personnage.frameHeight,
        personnage.frameWidth, personnage.frameHeight
    };

    SDL_Rect destRect = { x, y, personnage.frameWidth, personnage.frameHeight };

    SDL_RenderCopy(renderer, personnage.texture, &srcRect, &destRect);
}

void obtenirPositionPersonnage(int* x, int* y) {
    *x = personnage.x;
    *y = personnage.y;
}

int obtenirViePersonnage() {
    return personnage.vie_actuelle;
}

void subirDegatsPersonnage(int degats) {
    if (personnage.invincibilite <= 0) {
        personnage.vie_actuelle -= degats;
        personnage.invincibilite = 60; // ~1 seconde d'invincibilité
        logMessage("Le personnage subit %d dégâts! Vie restante: %d", degats, personnage.vie_actuelle);
    }
}

int estPersonnageVivant() {
    return personnage.vie_actuelle > 0;
}

void attaquerZombies() {
    for (int i = 0; i < nombre_zombies; i++) {
        int dx = zombies[i]->x - personnage.x;
        int dy = zombies[i]->y - personnage.y;
        float distance = sqrt(dx * dx + dy * dy);
    }
}

void fermerPersonnage() {
    if (personnage.texture) {
        SDL_DestroyTexture(personnage.texture);
        personnage.texture = NULL;
        logMessage("Texture du personnage fermée.");
    }
}

void dessinerBarreDeVie(SDL_Renderer* renderer, int x, int y, int largeur, int hauteur, int vie_actuelle, int vie_max) {
    SDL_Rect fond = { x, y, largeur, hauteur };
    SDL_Rect barre = { x, y, (int)((double)largeur * vie_actuelle / vie_max), hauteur };

    // Dessiner le fond de la barre de vie (rouge)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &fond);

    // Dessiner la barre de vie (verte)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &barre);
}