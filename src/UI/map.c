#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"

int chargerCarte(Jeu* jeu) {
    for (int i = 0; i < jeu->map.taille; i++) {
        for (int j = 0; j < jeu->map.taille; j++) {
            const char* cheminTexture = jeu->map.cases[i][j].texture_path;
            SDL_Texture* texture = obtenirTexture(jeu->renderer, cheminTexture);
            if (!texture) {
                return 0;
            }

            jeu->map.cases[i][j].texture = texture;

            if(jeu->map.cases[i][j].structure.texture_path) {
                SDL_Texture* structureTexture = obtenirTexture(jeu->renderer, jeu->map.cases[i][j].structure.texture_path);
                jeu->map.cases[i][j].structure.texture = structureTexture;
            }
        }
    }

    jeu->largeurCarte = jeu->map.taille * LARGEUR_CASE;
    jeu->hauteurCarte = jeu->map.taille * HAUTEUR_CASE;
    return 1;
}

void chargerTextureChunk(chunk *c, SDL_Renderer *renderer) {
    if (!c->loaded && !c->texture) {
        SDL_Surface *surface = IMG_Load(c->texture_path);
        if (!surface) {
            return;
        }

        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
        c->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (c->texture) {
            SDL_SetTextureBlendMode(c->texture, SDL_BLENDMODE_BLEND);
            c->loaded = 1;
        }
    }
}

float light_calculator(Jeu *jeu, int i, int j) {
    float time = fmod(jeu->countdown.elapsed_time / 60.0, 24.0);

    float light = 1;

    if (time > 6 && time < 8) {
        light = 0.2 + 0.8 * (time - 6) / 2;
    } else if (time > 18 && time < 20) {
        light = 1 - 0.8 * (time - 18) / 2;
    } else if (time >= 8 && time <= 18) {
        light = 1;
    } else {
        light = 0.2;
    }

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

void afficher_chunk(chunk *currentChunk, Jeu *jeu, int i, int j, float light) {
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

void afficher_structure(Jeu *jeu, int i, int j, float light) {

    Uint8 r = jeu->map.cases[i][j].brightness_R * light;
    Uint8 g = jeu->map.cases[i][j].brightness_G * light;
    Uint8 b = jeu->map.cases[i][j].brightness_B * light;

    appliquerFiltreCouleur(jeu->renderer, jeu->map.cases[i][j].structure.texture, r, g, b);

    SDL_Rect structureDest = {
        jeu->carteX + j * LARGEUR_CASE + (LARGEUR_CASE - jeu->map.cases[i][j].structure.width),
        jeu->carteY + i * HAUTEUR_CASE + (HAUTEUR_CASE - jeu->map.cases[i][j].structure.height),
        jeu->map.cases[i][j].structure.width,
        jeu->map.cases[i][j].structure.height
    };

    if (structureDest.x + structureDest.w >= 0 && structureDest.x < jeu->largeurEcran &&
        structureDest.y + structureDest.h >= 0 && structureDest.y < jeu->hauteurEcran) {
        if (jeu->map.cases[i][j].structure.texture) {
            SDL_RenderCopy(jeu->renderer, jeu->map.cases[i][j].structure.texture, NULL, &structureDest);
        }
    }
}

void appliquerFiltreCouleur(SDL_Renderer *renderer, SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetTextureColorMod(texture, r, g, b);
}