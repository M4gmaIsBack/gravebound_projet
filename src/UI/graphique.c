#include "graphique.h"
#include "../logs/logging.h"
#include "../controller/controller.h"
#include "../map/procedural.h"

// Initialise  SDL
// Configure la fenetre, charge la map
// Retourne 1 si succes, 0 en cas d'echec
int initGraphique(Jeu *jeu) {

    jeu->map = creerCarte(200);
    jeu->map = genererCarte(jeu->map);
    if(!enregistrerCarte(jeu->map)){
        logMessage("Erreur enregistrement carte");
        return 0;
    } else {
        logMessage("Carte enregistrée");
    }

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
    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            SDL_Surface *surface = IMG_Load(jeu->map.cases[i][j].texture_path);
            if (!surface) {
                logMessage("Erreur chargement image %s: %s", 
                            jeu->map.cases[i][j].texture_path, IMG_GetError());
                return 0;
            }

            jeu->map.cases[i][j].texture = SDL_CreateTextureFromSurface(jeu->renderer, surface);
            SDL_FreeSurface(surface); // Libérer immédiatement après utilisation

            if (!jeu->map.cases[i][j].texture) {
                logMessage("Erreur création texture %s: %s", 
                            jeu->map.cases[i][j].texture_path, SDL_GetError());
                
                // Libérer les textures déjà créées
                for (int k = 0; k <= i; k++) {
                    for (int l = 0; l < (k == i ? j : jeu->map.taille); l++) {
                        if (jeu->map.cases[k][l].texture) {
                            SDL_DestroyTexture(jeu->map.cases[k][l].texture);
                        }
                    }
                }
                return 0;
            }
        }
    }
    jeu->largeurCarte = jeu->map.taille;
    jeu->hauteurCarte = jeu->map.taille;
    return 1;
}

// Fonction pour appliquer un filtre de couleur (assombrir ou éclaircir)
void appliquerFiltreCouleur(SDL_Renderer *renderer, SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b) {
    // Applique une modification de couleur sur la texture
    SDL_SetTextureColorMod(texture, r, g, b);
}

// Libere toutes les ressources graphiques
void fermerGraphique(Jeu *jeu) {
    // Libération des textures des cases
    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            if (jeu->map.cases[i][j].texture) {
                SDL_DestroyTexture(jeu->map.cases[i][j].texture);
            }
        }
    }

    // Destruction du renderer
    if (jeu->renderer) {
        SDL_DestroyRenderer(jeu->renderer);
        jeu->renderer = NULL;
    }

    // Destruction de la fenêtre
    if (jeu->window) {
        SDL_DestroyWindow(jeu->window);
        jeu->window = NULL;
    }

    SDL_Quit();
}

// Dessine la carte et le personnage avec un filtre de couleur
void majRendu(Jeu *jeu) {
    // Efface l'écran avec un fond noir
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

    // Définir les couleurs du filtre (exemple: assombrir)


    // Dessine chaque case de la carte avec un filtre de couleur
    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            // Applique le filtre de couleur avant de dessiner

            Uint8 r = jeu->map.cases[i][j].brightness_R; // Rouge
            Uint8 g = jeu->map.cases[i][j].brightness_G; // Vert
            Uint8 b = jeu->map.cases[i][j].brightness_B; // Bleu

            appliquerFiltreCouleur(jeu->renderer, jeu->map.cases[i][j].texture, r, g, b);
            
            SDL_Rect dest = {
                jeu->carteX + j * LARGEUR_CASE, 
                jeu->carteY + i * HAUTEUR_CASE,
                LARGEUR_CASE, 
                HAUTEUR_CASE
            };
            SDL_RenderCopy(jeu->renderer, jeu->map.cases[i][j].texture, NULL, &dest);
        }
    }

    // Dessine un point rouge représentant le personnage
    SDL_SetRenderDrawColor(jeu->renderer, 255, 0, 0, 255);
    SDL_Rect personnageRect = {
        LARGEUR_ECRAN / 2 - 5,
        HAUTEUR_ECRAN / 2 - 5,
        10,
        10
    };
    SDL_RenderFillRect(jeu->renderer, &personnageRect);

    // Affiche le rendu à l'écran
    SDL_RenderPresent(jeu->renderer);
}
