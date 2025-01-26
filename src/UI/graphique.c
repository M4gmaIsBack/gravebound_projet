#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"
#include "../entities/character.h"
#include "../map/procedural.h"
#include "../entities/attack.h"
#include "cache.h"
#include "map.h"
#include "../entities/zombies.h"
#include "../UI/minimap.h"
#include "../buildings/base.h"


int initGraphique(Jeu *jeu) {
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

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");  // Pixel art mode
    SDL_RenderSetLogicalSize(jeu->renderer, jeu->largeurEcran, jeu->hauteurEcran);
    SDL_SetRenderDrawBlendMode(jeu->renderer, SDL_BLENDMODE_BLEND);

    // Initialize base coordinates
    jeu->base.x = jeu->largeurEcran / 2;
    jeu->base.y = jeu->hauteurEcran / 2;
    return 1;
}

void init_carte(Jeu *jeu, char *save) {
    GenerationParams params = chargerParams(save);
    jeu->map = creerCarte(params.taille);
    jeu->map = genererCarte(jeu->map, params);

    jeu->base = init_base(jeu);

    // Set base coordinates to the center of the map
    jeu->base.x = (jeu->map.taille * LARGEUR_CASE) / 2;
    jeu->base.y = (jeu->map.taille * HAUTEUR_CASE) / 2;

    if (!chargerCarte(jeu)) {
        logMessage("Erreur chargement carte");
        return;
    }
}

void fermerGraphique(Jeu *jeu) {
    free(jeu->map.cases);
    free(jeu->map.regions);

    logMessage("Textures de la carte libérées");

    fermerPersonnage();

    logMessage("texture du personnage libéré");

    nettoyer_zombies();

    logMessage("Zombies nettoyés");

    libererCache();

    logMessage("Cache libéré");

    if (jeu->renderer) SDL_DestroyRenderer(jeu->renderer);
    if (jeu->window) SDL_DestroyWindow(jeu->window);

    SDL_Quit();
    logMessage("Ressources graphiques libérées");
}

void majRendu(Jeu *jeu) {
    // Clear avec noir
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

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

    int centreEcranX = jeu->largeurEcran / 2;
    int centreEcranY = jeu->hauteurEcran / 2;
    
    int joueurCarteX = centreEcranX - jeu->carteX;
    int joueurCarteY = centreEcranY - jeu->carteY;

    render_attacks(jeu->renderer, jeu, joueurCarteX, joueurCarteY);
    
    mettre_a_jour_zombies(jeu);
    
    SDL_Texture* zombieTexture = obtenirTexture(jeu->renderer, "./assets/zombies/zombies_spritesheet.png");
    if (zombieTexture) {
        afficher_zombies(jeu, zombieTexture, joueurCarteX, joueurCarteY, centreEcranX, centreEcranY);
    }

    SDL_Texture* baseTexture = obtenirTexture(jeu->renderer, "./assets/map/artefacts/house1.png");
    if (baseTexture) {
        afficher_base(jeu, baseTexture, &jeu->base);
    }

    afficherMinimap(jeu);

    SDL_RenderPresent(jeu->renderer);
}

