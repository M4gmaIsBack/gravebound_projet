#include "character.h"
#include "../logs/logging.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// La texture du personnage
SDL_Texture* personnageTexture = NULL;

// Charge l'image du personnage en memoire
// Convertit l'image en texture '(pour le render)'
// Retourne 1 si succes, 0 en cas d'echec
int chargerPersonnage(SDL_Renderer* renderer) {
    // Chargement de l'image depuis le fichier
    SDL_Surface* surface = IMG_Load("./assets/character/character.png");
    if (!surface) {
        logMessage("Erreur de chargement du personnage: %s", IMG_GetError());
        return 0;
    }

    // Conversion de la surface en texture
    personnageTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Verification de la creation de texture
    if (!personnageTexture) {
        logMessage("Erreur de création de la texture du personnage: %s", SDL_GetError());
        return 0;
    }

    logMessage("Personnage charger.");
    return 1;
}

// Dessine le personnage a l'ecran aux coordonnees specifiques
void dessinerPersonnage(SDL_Renderer* renderer, int carteX, int carteY) {
    if (!personnageTexture) {
        logMessage("Erreur: le personnage n'a pas été charger.");
        return;
    }

    // Taille du personnage
    SDL_Rect destRect = { carteX, carteY, 50, 50 };  
    SDL_RenderCopy(renderer, personnageTexture, NULL, &destRect);
}

void fermerPersonnage() {
    // Destruction de la texture si elle existe
    if (personnageTexture) {
        SDL_DestroyTexture(personnageTexture);
        personnageTexture = NULL;
        logMessage("Texture du personnage fermée.");
    }
}