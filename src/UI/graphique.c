#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"

// Initialise  SDL
// Configure la fenetre, charge la map
// Retourne 1 si succes, 0 en cas d'echec
int initGraphique(Jeu *jeu) {
    // Initialisation des modules SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        logMessage("Erreur SDL init: %s", SDL_GetError());
        return 0;
    }

    // Creation de la fenetre principale
    jeu->window = SDL_CreateWindow(
        "Gravebound",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR_ECRAN,
        HAUTEUR_ECRAN,
        SDL_WINDOW_SHOWN
    );

    if (!jeu->window) {
        logMessage("Erreur création fenetre: %s", SDL_GetError());
        return 0;
    }

    // Creation du renderer
    jeu->renderer = SDL_CreateRenderer(jeu->window, -1, SDL_RENDERER_ACCELERATED);
    if (!jeu->renderer) {
        logMessage("Erreur création renderer: %s", SDL_GetError());
        return 0;
    }

    // Initialisation des coordonnees de la carte
    jeu->carteX = 0;
    jeu->carteY = 0;

    // Chargement de la map
    if (!chargerCarte(jeu)) {
        logMessage("Erreur chargement carte");
        return 0;
    }

    return 1;
}

// Charge la texture de la carte depuis un fichier image
// Initialise les dimensions de la carte
// Retourne 1 si succes, 0 en cas d'echec
int chargerCarte(Jeu *jeu) {
    // Chargement de l'image de la carte
    SDL_Surface *surface = IMG_Load("./assets/map/map_gravebound_1.png");
    if (!surface) {
        logMessage("Erreur chargement image: %s", IMG_GetError());
        return 0;
    }

    // Creation de la texture a partir de l'image
    jeu->carteTexture = SDL_CreateTextureFromSurface(jeu->renderer, surface);
    SDL_QueryTexture(jeu->carteTexture, NULL, NULL, &jeu->largeurCarte, &jeu->hauteurCarte);

    SDL_FreeSurface(surface);

    if (!jeu->carteTexture) {
        logMessage("Erreur création texture: %s", SDL_GetError());
        return 0;
    }

    return 1;
}

// Libere toutes les ressources graphiques
void fermerGraphique(Jeu *jeu) {
    // Destruction de la texture de carte
    if (jeu->carteTexture) {
        SDL_DestroyTexture(jeu->carteTexture);
        jeu->carteTexture = NULL;
    }

    // Destruction du renderer
    if (jeu->renderer) {
        SDL_DestroyRenderer(jeu->renderer);
        jeu->renderer = NULL;
    }

    // Destruction de la fenetre
    if (jeu->window) {
        SDL_DestroyWindow(jeu->window);
        jeu->window = NULL;
    }

    SDL_Quit();
}

// Dessine la carte et le personnage
void majRendu(Jeu *jeu) {
    // Efface l'ecran avec un fond noir
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

    // Dessine la carte
    SDL_Rect carteRect = {
        jeu->carteX,
        jeu->carteY,
        jeu->largeurCarte,
        jeu->hauteurCarte
    };
    SDL_RenderCopy(jeu->renderer, jeu->carteTexture, NULL, &carteRect);

    // Dessine un point rouge representant le personnage
    SDL_SetRenderDrawColor(jeu->renderer, 255, 0, 0, 255);
    SDL_Rect personnageRect = {
        LARGEUR_ECRAN/2 - 5,
        HAUTEUR_ECRAN/2 - 5,
        10,
        10
    };
    SDL_RenderFillRect(jeu->renderer, &personnageRect);

    // Affiche le rendu a l'ecran
    SDL_RenderPresent(jeu->renderer);
}