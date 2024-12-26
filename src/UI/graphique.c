#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"
#include "../entities/character.h"
#include "../map/procedural.h"

#define FPS 60
#define FRAME_DELAY (1000 / FPS)

typedef struct TextureCache {
    char* chemin;
    SDL_Texture* texture;
    struct TextureCache* suivant;
} TextureCache;

static TextureCache* cache = NULL;

SDL_Texture* obtenirTexture(SDL_Renderer* renderer, const char* chemin) {
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


int initGraphique(Jeu *jeu) {
    // Création et génération de la carte
    jeu->map = creerCarte(2000);
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

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

void chargerTextureChunk(chunk *c, SDL_Renderer *renderer) {
    if (!c->loaded && !c->texture) {
        SDL_Surface *surface = IMG_Load(c->texture_path);
        if (!surface) {
            return;
        }

        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
        c->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        c->loaded = 1; // Marquez le chunk comme chargé
    }
}

float light_calculator(Jeu *jeu, int i, int j) {
    float light = 1;
    if (jeu->map.cases[i][j].region != 5) {
        for (int k = -10; k < 10 && i + k < jeu->map.taille; k++) {
            for (int l = -10; l < 10 && j + l < jeu->map.taille; l++) {
                if (jeu->map.cases[i + k][j + l].region == 5) {
                    float distance = sqrt(k * k + l * l);
                    if (distance < 10) {
                        light += 1 - distance / 10;
                    } 
                }
            }
        }
    } else {
        light = 1.0;
    }
    light = fmin(light, 1.0);
    return light;
}

void appliquerFiltreCouleur(SDL_Renderer *renderer, SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetTextureColorMod(texture, r, g, b);
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

            if (!currentChunk->loaded) {
                chargerTextureChunk(currentChunk, jeu->renderer);
            }

            Uint8 r = jeu->map.cases[i][j].brightness_R * light;
            Uint8 g = jeu->map.cases[i][j].brightness_G * light;
            Uint8 b = jeu->map.cases[i][j].brightness_B * light;

            appliquerFiltreCouleur(jeu->renderer, jeu->map.cases[i][j].texture, r, g, b);

            SDL_Rect dest = {
                jeu->carteX + j * LARGEUR_CASE,
                jeu->carteY + i * HAUTEUR_CASE,
                LARGEUR_CASE,
                HAUTEUR_CASE
            };

            if (dest.x + dest.w >= 0 && dest.x < jeu->largeurEcran &&
                dest.y + dest.h >= 0 && dest.y < jeu->hauteurEcran) {
                if (currentChunk->texture) {
                    SDL_RenderCopy(jeu->renderer, currentChunk->texture, NULL, &dest);
                } else {
                    SDL_SetRenderDrawColor(jeu->renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(jeu->renderer, &dest);
                }
            }
        }
    }

    // Rendu du personnage
    const Uint8* state = SDL_GetKeyboardState(NULL);
    mettreAJourPersonnage(state);
    dessinerPersonnage(jeu->renderer, jeu->largeurEcran / 2 - 16, jeu->hauteurEcran / 2 - 24);

    SDL_RenderPresent(jeu->renderer);
}

void toggleFullscreen(Jeu *jeu) {
    Uint32 flags = SDL_GetWindowFlags(jeu->window);
    if (flags & SDL_WINDOW_FULLSCREEN) {
        SDL_SetWindowFullscreen(jeu->window, 0);
        logMessage("Basculé en mode fenêtré");
    } else {
        SDL_SetWindowFullscreen(jeu->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        logMessage("Basculé en mode plein écran");
    }

    // Mettre à jour les dimensions de la fenêtre
    SDL_GetWindowSize(jeu->window, &jeu->largeurEcran, &jeu->hauteurEcran);
    logMessage("Dimensions mises à jour : %dx%d", jeu->largeurEcran, jeu->hauteurEcran);
}

void gererInputManette(Jeu *jeu, SDL_Event *event) {
    switch(event->type) {
        case SDL_CONTROLLERBUTTONDOWN:
            if (event->cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                toggleFullscreen(jeu);
            }
            break;
        case SDL_WINDOWEVENT:
            switch (event->window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                case SDL_WINDOWEVENT_MINIMIZED:
                case SDL_WINDOWEVENT_RESTORED:
                case SDL_WINDOWEVENT_RESIZED:
                    break;
                default:
                    break;
            }
            break;
    }
}
