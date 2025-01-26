#include "minimap.h"
#include "../entities/zombies.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>

void afficherMinimap(Jeu *jeu) {
    static SDL_Texture *circleTexture = NULL;

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

    int tailleMinimap = 200;
    int rayon = tailleMinimap / 2;
    int centreX = rayon;
    int centreY = rayon;

    SDL_Texture *minimapTexture = SDL_CreateTexture(
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

    SDL_SetRenderTarget(jeu->renderer, minimapTexture);
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 0);
    SDL_RenderClear(jeu->renderer);

    float scale = (float)rayon / (jeu->map.taille / 10.0f);

    int joueurCarteX = (jeu->largeurEcran / 2 - jeu->carteX) / LARGEUR_CASE;
    int joueurCarteY = (jeu->hauteurEcran / 2 - jeu->carteY) / HAUTEUR_CASE;

    for (int i = joueurCarteY - 20; i <= joueurCarteY + 20; i++) {
        for (int j = joueurCarteX - 20; j <= joueurCarteX + 20; j++) {
            if (i < 0 || i >= jeu->map.taille || j < 0 || j >= jeu->map.taille) {
                continue;
            }

            chunk *currentChunk = &jeu->map.cases[i][j];

            int minimapX = centreX + (j - joueurCarteX) * LARGEUR_CASE * scale;
            int minimapY = centreY + (i - joueurCarteY) * HAUTEUR_CASE * scale;

            if ((minimapX - centreX) * (minimapX - centreX) + (minimapY - centreY) * (minimapY - centreY) <= rayon * rayon) {
                SDL_SetRenderDrawColor(
                    jeu->renderer,
                    currentChunk->brightness_R,
                    currentChunk->brightness_G,
                    currentChunk->brightness_B,
                    255
                );
                SDL_RenderFillRect(jeu->renderer, &(SDL_Rect){minimapX, minimapY, ceil(LARGEUR_CASE * scale), ceil(HAUTEUR_CASE * scale)});
            }
        }
    }

    SDL_SetRenderDrawColor(jeu->renderer, 255, 0, 0, 255);
    for (int i = 0; i < nombre_zombies; i++) {
        int zombieMinimapX = centreX + (zombies[i]->x - (joueurCarteX * LARGEUR_CASE)) * scale / LARGEUR_CASE;
        int zombieMinimapY = centreY + (zombies[i]->y - (joueurCarteY * HAUTEUR_CASE)) * scale / HAUTEUR_CASE;

        if ((zombieMinimapX - centreX) * (zombieMinimapX - centreX) + (zombieMinimapY - centreY) * (zombieMinimapY - centreY) <= rayon * rayon) {
            SDL_RenderFillRect(jeu->renderer, &(SDL_Rect){zombieMinimapX - 2, zombieMinimapY - 2, 4, 4});
        }
    }

    SDL_SetRenderDrawColor(jeu->renderer, 0, 255, 0, 255);
    SDL_Rect playerRect = {centreX - 2, centreY - 2, 4, 4};
    SDL_RenderFillRect(jeu->renderer, &playerRect);

    SDL_SetRenderTarget(jeu->renderer, NULL);

    SDL_Rect minimapDestRect = {20, jeu->hauteurEcran - tailleMinimap - 20, tailleMinimap, tailleMinimap};
    SDL_RenderCopy(jeu->renderer, minimapTexture, NULL, &minimapDestRect);

    SDL_Rect destRect = {10, jeu->hauteurEcran - tailleMinimap - 30, tailleMinimap + 40, tailleMinimap + 30};
    SDL_RenderCopy(jeu->renderer, circleTexture, NULL, &destRect);

    SDL_DestroyTexture(minimapTexture);
}
