#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"
#include "../entities/character.h"
#include "../entities/character.c"

// Initialise SDL
// Configure la fenêtre, charge la carte et le personnage
// Retourne 1 si succès, 0 en cas d'échec
int initGraphique(Jeu *jeu) {
    // Initialisation des modules SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        logMessage("Erreur SDL init: %s", SDL_GetError());
        return 0;
    }

    // Création de la fenêtre principale
    jeu->window = SDL_CreateWindow(
        "Gravebound",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR_ECRAN,
        HAUTEUR_ECRAN,
        SDL_WINDOW_SHOWN
    );

    if (!jeu->window) {
        logMessage("Erreur création fenêtre: %s", SDL_GetError());
        return 0;
    }

    // Création du renderer
    jeu->renderer = SDL_CreateRenderer(jeu->window, -1, SDL_RENDERER_ACCELERATED);
    if (!jeu->renderer) {
        logMessage("Erreur création renderer: %s", SDL_GetError());
        return 0;
    }

    // Initialisation des coordonnées de la carte
    jeu->carteX = 0;
    jeu->carteY = 0;

    // Chargement de la carte
    if (!chargerCarte(jeu)) {
        logMessage("Erreur chargement carte");
        return 0;
    }

    // Chargement du personnage
    if (!chargerPersonnage(jeu->renderer)) {
        logMessage("Erreur chargement personnage");
        return 0;
    }

    return 1;
}

// Charge la texture de la carte depuis un fichier image
// Retourne 1 si succès, 0 en cas d'échec
int chargerCarte(Jeu *jeu) {
    SDL_Surface *surface = IMG_Load("./assets/map/map_gravebound_1.png");
    if (!surface) {
        logMessage("Erreur chargement image: %s", IMG_GetError());
        return 0;
    }

    jeu->carteTexture = SDL_CreateTextureFromSurface(jeu->renderer, surface);
    SDL_QueryTexture(jeu->carteTexture, NULL, NULL, &jeu->largeurCarte, &jeu->hauteurCarte);
    SDL_FreeSurface(surface);

    if (!jeu->carteTexture) {
        logMessage("Erreur création texture: %s", SDL_GetError());
        return 0;
    }

    return 1;
}

// Libère toutes les ressources graphiques
void fermerGraphique(Jeu *jeu) {
    if (jeu->carteTexture) {
        SDL_DestroyTexture(jeu->carteTexture);
        jeu->carteTexture = NULL;
    }

    fermerPersonnage(); // Libère la texture du personnage

    if (jeu->renderer) {
        SDL_DestroyRenderer(jeu->renderer);
        jeu->renderer = NULL;
    }

    if (jeu->window) {
        SDL_DestroyWindow(jeu->window);
        jeu->window = NULL;
    }

    SDL_Quit();
}

// Met à jour le rendu : dessine la carte et le personnage
void majRendu(Jeu *jeu) {
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

    // Dessine la carte
    SDL_Rect carteRect = {
        jeu->carteX, jeu->carteY, jeu->largeurCarte, jeu->hauteurCarte
    };
    SDL_RenderCopy(jeu->renderer, jeu->carteTexture, NULL, &carteRect);

    // Récupère l'état du clavier
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Met à jour et dessine le personnage
    mettreAJourPersonnage(state);
    dessinerPersonnage(jeu->renderer, LARGEUR_ECRAN / 2 - 16, HAUTEUR_ECRAN / 2 - 24);

    SDL_RenderPresent(jeu->renderer);
}