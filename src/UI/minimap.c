#include "minimap.h"
#include "../map/procedural.h"
#include "../entities/zombies.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>

// Fonction pour afficher la minimap
void afficherMinimap(Jeu *jeu) {
    static SDL_Texture *circleTexture = NULL;
    static SDL_Texture *minimapTexture = NULL;

    // Charger l'image circulaire si elle n'est pas encore chargée
    if (!circleTexture) {
        SDL_Surface *surface = IMG_Load("./assets/minimap/circle.png");
        if (!surface) {
            fprintf(stderr, "Erreur : Impossible de charger l'image circle.png : %s\n", IMG_GetError());
            return;
        }

        circleTexture = SDL_CreateTextureFromSurface(jeu->renderer, surface);
        SDL_FreeSurface(surface);

        if (!circleTexture) {
            fprintf(stderr, "Erreur : Impossible de créer une texture pour circle.png : %s\n", SDL_GetError());
            return;
        }
    }

    // Créer une texture pour la minimap (si pas encore créée)
    int tailleMinimap = 200; // Taille en pixels
    if (!minimapTexture) {
        minimapTexture = SDL_CreateTexture(
            jeu->renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            tailleMinimap,
            tailleMinimap
        );

        if (!minimapTexture) {
            fprintf(stderr, "Erreur : Impossible de créer la texture de la minimap : %s\n", SDL_GetError());
            return;
        }
    }

    // Mettre à jour la minimap
    SDL_SetRenderTarget(jeu->renderer, minimapTexture);
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 255);
    SDL_RenderClear(jeu->renderer);

    // Échelle pour réduire la carte
        float zoomFactor = 50.5f; // Ajustez cette valeur : >1 pour dézoomer, <1 pour zoomer
        float scale = (float)tailleMinimap / (jeu->map.taille * LARGEUR_CASE * zoomFactor);

    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            chunk *currentChunk = &jeu->map.cases[i][j];

            // Rectangle pour le chunk
            SDL_Rect chunkRect = {
                j * LARGEUR_CASE * scale,
                i * HAUTEUR_CASE * scale,
                ceil(LARGEUR_CASE * scale),
                ceil(HAUTEUR_CASE * scale)
            };

            // Dessiner la texture du chunk si disponible
            if (currentChunk->texture) {
                SDL_RenderCopy(jeu->renderer, currentChunk->texture, NULL, &chunkRect);
            } else {
                SDL_SetRenderDrawColor(
                    jeu->renderer,
                    currentChunk->brightness_R,
                    currentChunk->brightness_G,
                    currentChunk->brightness_B,
                    255
                );
                SDL_RenderFillRect(jeu->renderer, &chunkRect);
            }
        }
    }

    // Dessiner la minimap sur le cercle
    SDL_SetRenderTarget(jeu->renderer, NULL);
    SDL_Rect destRect = { 20, jeu->hauteurEcran - tailleMinimap - 20, tailleMinimap, tailleMinimap };
    SDL_RenderCopy(jeu->renderer, minimapTexture, NULL, &destRect);

    // Appliquer le masque circulaire
    SDL_RenderCopy(jeu->renderer, circleTexture, NULL, &destRect);

    // Dessiner la position du joueur sur la minimap
    int joueurX = jeu->carteX + jeu->largeurEcran / 2;
    int joueurY = jeu->carteY + jeu->hauteurEcran / 2;
    int joueurMinimapX = 20 + (joueurX * scale / LARGEUR_CASE);
    int joueurMinimapY = jeu->hauteurEcran - tailleMinimap - 20 + (joueurY * scale / HAUTEUR_CASE);

    SDL_SetRenderDrawColor(jeu->renderer, 255, 255, 255, 255); // Blanc pour le joueur
    SDL_Rect playerRect = { joueurMinimapX - 2, joueurMinimapY - 2, 4, 4 };
    SDL_RenderFillRect(jeu->renderer, &playerRect);
}
