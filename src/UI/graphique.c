#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"

// Initialise SDL, configure la fenêtre et charge la carte
int initGraphique(Jeu *jeu) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        logMessage("Erreur SDL init: %s", SDL_GetError());
        return 0;
    }

    if ((IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)) != (IMG_INIT_PNG | IMG_INIT_JPG)) {
        logMessage("Erreur initialisation SDL_image: %s", IMG_GetError());
        return 0;
    }

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        logMessage("Erreur récupération résolution: %s", SDL_GetError());
        return 0;
    }

    jeu->window = SDL_CreateWindow(
        "Gravebound",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        displayMode.w,
        displayMode.h,
        SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    if (!jeu->window) {
        logMessage("Erreur création fenêtre: %s", SDL_GetError());
        return 0;
    }

    jeu->renderer = SDL_CreateRenderer(jeu->window, -1, SDL_RENDERER_ACCELERATED);
    if (!jeu->renderer) {
        logMessage("Erreur création renderer: %s", SDL_GetError());
        return 0;
    }

    SDL_GetWindowSize(jeu->window, &jeu->largeurEcran, &jeu->hauteurEcran);
    logMessage("Fenêtre initialisée en plein écran : %dx%d", jeu->largeurEcran, jeu->hauteurEcran);

    // Initialisation de la manette
    if (!initManette()) {
        logMessage("Manette non détectée ou échec d'initialisation");
    }

    return 1;
}

// Charge la texture de la carte
int chargerCarte(Jeu *jeu) {
    SDL_Surface *surface = IMG_Load("./assets/map/map_gravebound_1.png");
    if (!surface) {
        logMessage("Erreur chargement image carte : %s", IMG_GetError());
        return 0;
    }

    jeu->carteTexture = SDL_CreateTextureFromSurface(jeu->renderer, surface);
    SDL_FreeSurface(surface);

    if (!jeu->carteTexture) {
        logMessage("Erreur création texture carte : %s", SDL_GetError());
        return 0;
    }

    SDL_QueryTexture(jeu->carteTexture, NULL, NULL, &jeu->largeurCarte, &jeu->hauteurCarte);
    jeu->carteX = 0;
    jeu->carteY = 0;

    logMessage("Carte chargée avec succès : %dx%d", jeu->largeurCarte, jeu->hauteurCarte);
    return 1;
}


// Libère toutes les ressources graphiques
void fermerGraphique(Jeu *jeu) {
    if (jeu->carteTexture) {
        SDL_DestroyTexture(jeu->carteTexture);
        jeu->carteTexture = NULL;
    }
    if (jeu->renderer) {
        SDL_DestroyRenderer(jeu->renderer);
        jeu->renderer = NULL;
    }
    if (jeu->window) {
        SDL_DestroyWindow(jeu->window);
        jeu->window = NULL;
    }

    // Fermer la manette
    fermerManette();

    IMG_Quit();
    SDL_Quit();
    logMessage("Ressources graphiques libérées");
}

// Dessine la carte et le personnage
void majRendu(Jeu *jeu) {
    if (!jeu || !jeu->renderer) {
        logMessage("Erreur : Renderer ou structure Jeu invalide !");
        return;
    }

    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

    if (jeu->carteTexture) {
        SDL_Rect carteRect = {
            jeu->carteX,
            jeu->carteY,
            jeu->largeurCarte,
            jeu->hauteurCarte
        };
        if (SDL_RenderCopy(jeu->renderer, jeu->carteTexture, NULL, &carteRect) != 0) {
            logMessage("Erreur : SDL_RenderCopy sur carteTexture : %s", SDL_GetError());
        }
    } else {
        logMessage("Erreur : carteTexture invalide ou non chargée !");
    }

    SDL_RenderPresent(jeu->renderer);
    logMessage("Rendu mis à jour avec succès");
}


// Basculer entre plein écran et mode fenêtré
void toggleFullscreen(Jeu *jeu) {
    Uint32 flags = SDL_GetWindowFlags(jeu->window);
    if (flags & SDL_WINDOW_FULLSCREEN) {
        SDL_SetWindowFullscreen(jeu->window, 0); // Mode fenêtré
        logMessage("Basculé en mode fenêtré");
    } else {
        SDL_SetWindowFullscreen(jeu->window, SDL_WINDOW_FULLSCREEN_DESKTOP); // Plein écran
        logMessage("Basculé en mode plein écran");
    }
}

// Gestion de l'entrée via la manette pour basculer le mode plein écran
void gererInputManette(Jeu *jeu, SDL_Event *event) {
    if (event->type == SDL_CONTROLLERBUTTONDOWN) {
        if (event->cbutton.button == SDL_CONTROLLER_BUTTON_START) { // Bouton "Start" pour basculer
            toggleFullscreen(jeu);
        }
    }
}
