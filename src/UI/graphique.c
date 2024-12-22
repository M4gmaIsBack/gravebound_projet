#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"
#include "../entities/character.h"
#include "../map/procedural.h"

// Initialise SDL, configure la fenêtre et charge la carte
int initGraphique(Jeu *jeu) {
    // Génération et sauvegarde de la carte
    jeu->map = creerCarte(200);
    jeu->map = genererCarte(jeu->map);

    if (!enregistrerCarte(jeu->map)) {
        logMessage("Erreur enregistrement carte");
        return 0;
    }
    logMessage("Carte enregistrée");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        logMessage("Erreur SDL init: %s", SDL_GetError());
        return 0;
    }

    // Création de la fenetre en plein écran
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

    // Création du renderer
    jeu->renderer = SDL_CreateRenderer(jeu->window, -1, SDL_RENDERER_ACCELERATED);
    if (!jeu->window || !jeu->renderer) {
        logMessage("Erreur création fenêtre/renderer: %s", SDL_GetError());
        return 0;
    }

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

    jeu->carteX = 0;
    jeu->carteY = 0;

    return 1;
}

// Charge la texture de la carte
int chargerCarte(Jeu *jeu) {
    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            SDL_Surface *surface = IMG_Load(jeu->map.cases[i][j].texture_path);
            if (!surface) {
                logMessage("Erreur chargement image: %s", IMG_GetError());
                return 0;
            }

            jeu->map.cases[i][j].texture = SDL_CreateTextureFromSurface(jeu->renderer, surface);
            SDL_FreeSurface(surface);

            if (!jeu->map.cases[i][j].texture) {
                logMessage("Erreur création texture: %s", SDL_GetError());
                return 0;
            }
        }
    }

    jeu->largeurCarte = jeu->map.taille * LARGEUR_CASE;
    jeu->hauteurCarte = jeu->map.taille * HAUTEUR_CASE;
    return 1;
}


void fermerGraphique(Jeu *jeu) {
    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            if (jeu->map.cases[i][j].texture) {
                SDL_DestroyTexture(jeu->map.cases[i][j].texture);
            }
        }
    }

    fermerPersonnage();

    if (jeu->renderer) SDL_DestroyRenderer(jeu->renderer);
    if (jeu->window) SDL_DestroyWindow(jeu->window);

    SDL_Quit();
    logMessage("Ressources graphiques libérées");
}

// Met a jour le rendu, dessine la carte et le personnage
void majRendu(Jeu *jeu) {
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            SDL_Rect dest = {
                jeu->carteX + j * LARGEUR_CASE,
                jeu->carteY + i * HAUTEUR_CASE,
                LARGEUR_CASE,
                HAUTEUR_CASE
            };
            SDL_RenderCopy(jeu->renderer, jeu->map.cases[i][j].texture, NULL, &dest);
        }
    }

    // Mise a jour du personnage
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    mettreAJourPersonnage(state);
    dessinerPersonnage(jeu->renderer, jeu->largeurEcran / 2 - 16, jeu->hauteurEcran / 2 - 24);

    SDL_RenderPresent(jeu->renderer);
}

// Basculer entre plein écran et mode fenêtré
void toggleFullscreen(Jeu *jeu) {
    Uint32 flags = SDL_GetWindowFlags(jeu->window);
    if (flags & SDL_WINDOW_FULLSCREEN) {
        SDL_SetWindowFullscreen(jeu->window, 0);
        logMessage("Basculé en mode fenêtré");
    } else {
        SDL_SetWindowFullscreen(jeu->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        logMessage("Basculé en mode plein écran");
    }
}

void gererInputManette(Jeu *jeu, SDL_Event *event) {
    if (event->type == SDL_CONTROLLERBUTTONDOWN) {
        if (event->cbutton.button == SDL_CONTROLLER_BUTTON_START) {
            toggleFullscreen(jeu);
        }
    }
}
