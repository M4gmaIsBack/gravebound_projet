#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"
#include "../entities/character.h"
#include "../map/procedural.h"

// Initialise SDL
// Configure la fenêtre, charge la carte et le personnage
// Retourne 1 si succès, 0 en cas d'échec
int initGraphique(Jeu *jeu) {
    jeu->map = creerCarte(200);
    jeu->map = genererCarte(jeu->map);

    if (!enregistrerCarte(jeu->map)) {
        logMessage("Erreur enregistrement carte");
        return 0;
    } else {
        logMessage("Carte enregistrée");
    }

    // Initialisation SDL et fenêtres
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        logMessage("Erreur SDL init: %s", SDL_GetError());
        return 0;
    }

    jeu->window = SDL_CreateWindow("Gravebound", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR_ECRAN, HAUTEUR_ECRAN, SDL_WINDOW_SHOWN);
    jeu->renderer = SDL_CreateRenderer(jeu->window, -1, SDL_RENDERER_ACCELERATED);

    if (!jeu->window || !jeu->renderer) {
        logMessage("Erreur création fenêtre/renderer: %s", SDL_GetError());
        return 0;
    }

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

// Charge la texture de la carte depuis un fichier image ou générée
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

    jeu->largeurCarte = jeu->map.taille;
    jeu->hauteurCarte = jeu->map.taille;
    return 1;
}

// Libère toutes les ressources graphiques
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
}

// Met à jour le rendu
void majRendu(Jeu *jeu) {
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            SDL_Rect dest = {jeu->carteX + j * LARGEUR_CASE, jeu->carteY + i * HAUTEUR_CASE, LARGEUR_CASE, HAUTEUR_CASE};
            SDL_RenderCopy(jeu->renderer, jeu->map.cases[i][j].texture, NULL, &dest);
        }
    }

    dessinerPersonnage(jeu->renderer, LARGEUR_ECRAN / 2 - 16, HAUTEUR_ECRAN / 2 - 24);
    SDL_RenderPresent(jeu->renderer);
}
