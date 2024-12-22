#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"
#include "../entities/character.h"
#include "../map/procedural.h"

// Initialise SDL
// Configure la fenêtre, charge la carte et le personnage
// Retourne 1 si succès, 0 en cas d'échec
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"
#include "../entities/character.h"
#include "../map/procedural.h"

// Structure pour stocker une texture et son chemin
typedef struct TextureCache {
    char* chemin;
    SDL_Texture* texture;
    struct TextureCache* suivant;
} TextureCache;

static TextureCache* cache = NULL;

// Fonction pour obtenir une texture depuis le cache ou la charger
SDL_Texture* obtenirTexture(SDL_Renderer* renderer, const char* chemin) {
    // Rechercher dans le cache
    TextureCache* courant = cache;
    while (courant) {
        if (strcmp(courant->chemin, chemin) == 0) {
            return courant->texture;
        }
        courant = courant->suivant;
    }

    // Charger une nouvelle texture si elle n'est pas dans le cache
    SDL_Surface* surface = IMG_Load(chemin);
    if (!surface) {
        logMessage("Erreur chargement image: %s", IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        logMessage("Erreur création texture: %s", SDL_GetError());
        return NULL;
    }

    // Ajouter au cache
    TextureCache* nouvelElement = malloc(sizeof(TextureCache));
    nouvelElement->chemin = strdup(chemin);
    nouvelElement->texture = texture;
    nouvelElement->suivant = cache;
    cache = nouvelElement;

    return texture;
}

// Fonction pour libérer le cache
void libererCache() {
    TextureCache* courant = cache;
    while (courant) {
        TextureCache* suivant = courant->suivant;
        SDL_DestroyTexture(courant->texture);
        free(courant->chemin);
        free(courant);
        courant = suivant;
    }
    cache = NULL;
}

// Fonction pour charger les cases de la carte
int chargerCarte(Jeu* jeu) {
    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            const char* cheminTexture = jeu->map.cases[i][j].texture_path;
            SDL_Texture* texture = obtenirTexture(jeu->renderer, cheminTexture);
            if (!texture) {
                return 0;
            }

            jeu->map.cases[i][j].texture = texture;
        }
    }

    jeu->largeurCarte = jeu->map.taille * LARGEUR_CASE;
    jeu->hauteurCarte = jeu->map.taille * HAUTEUR_CASE;
    return 1;
}

// Libère toutes les ressources graphiques, y compris le cache
void fermerGraphique(Jeu* jeu) {
    // Libération des textures des cases
    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            jeu->map.cases[i][j].texture = NULL; // Texture partagée via le cache
        }
    }

    // Libération du cache
    libererCache();

    // Libération des autres ressources
    fermerPersonnage();

    if (jeu->renderer) SDL_DestroyRenderer(jeu->renderer);
    if (jeu->window) SDL_DestroyWindow(jeu->window);

    SDL_Quit();
}

// Initialisation graphique
int initGraphique(Jeu *jeu) {
    jeu->map = creerCarte(3000);
    jeu->map = genererCarte(jeu->map);

    if (!enregistrerCarte(jeu->map)) {
        logMessage("Erreur enregistrement carte");
        return 0;
    } else {
        logMessage("Carte enregistrée");
    }

    // Initialisation SDL et création des fenêtres
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

    // Initialisation des coordonnées de la carte
    jeu->carteX = (LARGEUR_ECRAN - jeu->largeurCarte) / 2;
    jeu->carteY = (HAUTEUR_ECRAN - jeu->hauteurCarte) / 2;

    return 1;
}


#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

void chargerTextureChunk(chunk *c, SDL_Renderer *renderer) {
    if (!c->loaded) {
        SDL_Surface *surface = IMG_Load(c->texture_path);
        if (!surface) {
            printf("Erreur de chargement de la texture : %s\n", IMG_GetError());
            return;
        }

        // Créez la texture originale (non modifiée)
        c->texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!c->texture) {
            printf("Erreur de création de la texture : %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            return;
        }

        SDL_FreeSurface(surface);
        c->loaded = 1; // Marquez le chunk comme chargé
    }
}

void majRendu(Jeu *jeu) {
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

    int blocsVisiblesX = LARGEUR_ECRAN / LARGEUR_CASE + 2;
    int blocsVisiblesY = HAUTEUR_ECRAN / HAUTEUR_CASE + 2;

    int debutX = MAX(0, -jeu->carteX / LARGEUR_CASE);
    int debutY = MAX(0, -jeu->carteY / HAUTEUR_CASE);
    int finX = MIN(jeu->map.taille, debutX + blocsVisiblesX);
    int finY = MIN(jeu->map.taille, debutY + blocsVisiblesY);

    for (int i = debutY; i < finY; i++) {
        for (int j = debutX; j < finX; j++) {
            chunk *currentChunk = &jeu->map.cases[i][j];

            // Charge la texture si elle n'est pas encore chargée
            if (!currentChunk->loaded) {
                chargerTextureChunk(currentChunk, jeu->renderer);
            }

            SDL_Rect dest = {
                jeu->carteX + j * LARGEUR_CASE,
                jeu->carteY + i * HAUTEUR_CASE,
                LARGEUR_CASE,
                HAUTEUR_CASE
            };

            if (dest.x + dest.w >= 0 && dest.x < LARGEUR_ECRAN &&
                dest.y + dest.h >= 0 && dest.y < HAUTEUR_ECRAN) {
                if (currentChunk->texture) {

                    SDL_RenderCopy(jeu->renderer, currentChunk->texture, NULL, &dest);

                    // Réinitialisez les couleurs après le rendu
                    SDL_SetTextureColorMod(currentChunk->texture, 255, 255, 255);
                } else {
                    SDL_SetRenderDrawColor(jeu->renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(jeu->renderer, &dest);
                }
            }
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    mettreAJourPersonnage(state);
    dessinerPersonnage(jeu->renderer, LARGEUR_ECRAN / 2 - 16, HAUTEUR_ECRAN / 2 - 24);

    SDL_RenderPresent(jeu->renderer);
}
