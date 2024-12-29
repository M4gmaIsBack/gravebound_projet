#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"
#include "../entities/character.h"
#include "../entities/zombies.h"
#include "cache.h"
#include "map.h"


int initGraphique(Jeu *jeu) {
    // Création et génération de la carte
    jeu->map = creerCarte(3000);
    jeu->map = genererCarte(jeu->map);

    if (!enregistrerCarte(jeu->map)) {
        logMessage("Erreur enregistrement carte");
        return 0;
    }
    logMessage("Carte enregistrée");

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        logMessage("Erreur SDL init: %s", SDL_GetError());
        return 0;
    }

    // Récupération de la résolution de l'écran
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        logMessage("Erreur récupération résolution: %s", SDL_GetError());
        return 0;
    }

    // Création de la fenêtre en plein écran
    jeu->window = SDL_CreateWindow(
        "Gravebound",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        displayMode.w,
        displayMode.h,
        SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    // Création du renderer
    jeu->renderer = SDL_CreateRenderer(jeu->window, -1, SDL_RENDERER_ACCELERATED);
    if (!jeu->window || !jeu->renderer) {
        logMessage("Erreur création fenêtre/renderer: %s", SDL_GetError());
        return 0;
    }

    // Récupération des dimensions actuelles de la fenêtre
    SDL_GetWindowSize(jeu->window, &jeu->largeurEcran, &jeu->hauteurEcran);
    logMessage("Fenêtre initialisée en plein écran : %dx%d", jeu->largeurEcran, jeu->hauteurEcran);

    // Initialisation de la manette
    if (!initManette()) {
        logMessage("Manette non détectée ou échec d'initialisation");
    }

    // Chargement du personnage
    if (!chargerPersonnage(jeu->renderer)) {
        logMessage("Erreur chargement personnage");
        return 0;
    }

    // Initialisation des coordonnées de la carte pour centrer la caméra sur le centre de la carte
    jeu->carteX = -(jeu->map.taille * LARGEUR_CASE / 2) + (jeu->largeurEcran / 2);
    jeu->carteY = -(jeu->map.taille * HAUTEUR_CASE / 2) + (jeu->hauteurEcran / 2);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");  // Pixel art mode
    SDL_RenderSetLogicalSize(jeu->renderer, jeu->largeurEcran, jeu->hauteurEcran);
    SDL_SetRenderDrawBlendMode(jeu->renderer, SDL_BLENDMODE_BLEND);
    
    // Précharger toutes les textures au démarrage
    if (!chargerCarte(jeu)) {
        logMessage("Erreur chargement carte");
        return 0;
    }

    // Initialiser les zombies avec des positions relatives au centre de la carte
    int centreX = (jeu->map.taille * LARGEUR_CASE) / 2;
    int centreY = (jeu->map.taille * HAUTEUR_CASE) / 2;
    initialiser_zombies_autour_position(10, centreX, centreY, 500); // 500 pixels autour du centre
    
    return 1;
}

void fermerGraphique(Jeu *jeu) {
    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            if (jeu->map.cases[i][j].texture) {
                SDL_DestroyTexture(jeu->map.cases[i][j].texture);
            }

            if (jeu->map.cases[i][j].structure.texture) {
                SDL_DestroyTexture(jeu->map.cases[i][j].structure.texture);
            }
        }
    }

    fermerPersonnage();

    if (jeu->renderer) SDL_DestroyRenderer(jeu->renderer);
    if (jeu->window) SDL_DestroyWindow(jeu->window);

    SDL_Quit();
    logMessage("Ressources graphiques libérées");
}

void majRendu(Jeu *jeu) {
    // Clear avec noir
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

    // Calculer la zone visible avec une marge plus petite
    int blocsVisiblesX = (jeu->largeurEcran / LARGEUR_CASE) + 2;
    int blocsVisiblesY = (jeu->hauteurEcran / HAUTEUR_CASE) + 2;

    int debutX = MAX(0, (-jeu->carteX / LARGEUR_CASE));
    int debutY = MAX(0, (-jeu->carteY / HAUTEUR_CASE));
    int finX = MIN(jeu->map.taille, debutX + blocsVisiblesX);
    int finY = MIN(jeu->map.taille, debutY + blocsVisiblesY);

    for (int i = debutY; i < finY; i++) {
        for (int j = debutX; j < finX; j++) {
            chunk *currentChunk = &jeu->map.cases[i][j];

            float light = light_calculator(jeu, i, j);

            afficher_chunk(currentChunk, jeu, i, j, light);

            if (jeu->map.cases[i][j].structure.texture_path) {

                afficher_structure(jeu, i, j, light);
            }
        }
    }

    // Rendu du personnage
    const Uint8* state = SDL_GetKeyboardState(NULL);
    mettreAJourPersonnage(state);
    dessinerPersonnage(jeu->renderer, jeu->largeurEcran / 2 - 16, jeu->hauteurEcran / 2 - 24);
    dessinerBarreDeVie(jeu->renderer, jeu->largeurEcran / 2 - 16, jeu->hauteurEcran / 2 - 32, 32, 5, obtenirViePersonnage(), 100);

    // Mettre à jour et rendre les zombies
    int centreEcranX = jeu->largeurEcran / 2;
    int centreEcranY = jeu->hauteurEcran / 2;
    
    // Position réelle du joueur sur la carte
    int joueurCarteX = centreEcranX - jeu->carteX;
    int joueurCarteY = centreEcranY - jeu->carteY;
    
    mettre_a_jour_zombies(joueurCarteX, joueurCarteY);
    
    SDL_Texture* zombieTexture = obtenirTexture(jeu->renderer, "./assets/zombies/zombies_spritesheet.png");
    if (zombieTexture) {
        afficher_zombies(jeu, zombieTexture, joueurCarteX, joueurCarteY, centreEcranX, centreEcranY);
    }

    SDL_RenderPresent(jeu->renderer);
}