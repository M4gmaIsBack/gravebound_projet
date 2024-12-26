#include "character.h"
#include "../logs/logging.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Structure pour le personnage
typedef struct {
    SDL_Texture* texture;      // Texture du personnage
    int frameWidth;            // Largeur d'une frame
    int frameHeight;           // Hauteur d'une frame
    int currentFrame;          // Frame actuelle
    int totalFrames;           // Nombre de frames par animation
    int direction;             // Direction actuelle (0 = bas, 1 = gauche, 2 = droite, 3 = haut)
    int moving;                // Indique si le personnage est en mouvement
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

    logMessage("Sprite-sheet du personnage chargée.");
    return 1;
}

void mettreAJourPersonnage(const Uint8* state) {
    personnage.moving = 0;

    // Détermine la direction selon les touches pressées (ZQSD)
    if (state[SDL_SCANCODE_W]) {
        personnage.direction = 3;
        personnage.moving = 1;
    } else if (state[SDL_SCANCODE_S]) {
        personnage.direction = 0;
        personnage.moving = 1;
    } else if (state[SDL_SCANCODE_A]) {
        personnage.direction = 1;
        personnage.moving = 1;
    } else if (state[SDL_SCANCODE_D]) { 
        personnage.direction = 2;
        personnage.moving = 1;
    }

    // Met à jour la frame uniquement si le personnage bouge
    if (personnage.moving) {
        personnage.currentFrame++;
        if (personnage.currentFrame >= personnage.totalFrames) {
            personnage.currentFrame = 0;
        }
    } else {
        personnage.currentFrame = 1;
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

void fermerPersonnage() {
    if (personnage.texture) {
        SDL_DestroyTexture(personnage.texture);
        personnage.texture = NULL;
        logMessage("Texture du personnage fermée.");
    }
}
