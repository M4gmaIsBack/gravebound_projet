#include <stdio.h>
#include <stdlib.h>
#include "menu_personnage.h"
#include "menu.h"  // Ajout de cette ligne
#include "../audio/audio.h"
#include "../logs/logging.h"

const char* cheminSpriteSheetPersonnage = "./assets/character/spritesheet_marcus.png";  // Initialisation par défaut

// Déclaration des fonctions
static void gererClavierMenuPersonnage(SDL_Event *e, int *selection, int *quitter, AudioAssets *audio);
static void renderMenuPersonnage(Game *game, MenuPersoAssets *assets, int selection);

static int chargerAssetsMenuPerso(SDL_Renderer *r, MenuPersoAssets *m) {
    // Charger le background
    SDL_Surface *surface = IMG_Load("./assets/menu/background_perso.jpg");
    if (!surface) {
        logMessage("Erreur load background perso: %s", IMG_GetError());
        return 0;
    }
    m->background = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);

    // Charger les personnages avec le nouveau chemin
    const char* paths[3] = {
        "./assets/character/spritesheet_marcus.png",
        "./assets/character/spritesheet_sarah.png",
        "./assets/character/spritesheet_alex.png"
    };

    for (int i = 0; i < 3; i++) {
        surface = IMG_Load(paths[i]);
        if (!surface) {
            logMessage("Erreur load perso %s: %s", paths[i], IMG_GetError());
            return 0;
        }
        m->persos[i] = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);
    }

    // Charger les flèches
    surface = IMG_Load("./assets/menu/fleche_gauche.png");
    if (!surface) {
        logMessage("Erreur load fleche gauche: %s", IMG_GetError());
        return 0;
    }
    m->flecheGauche = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./assets/menu/fleche_droit.png");
    if (!surface) {
        logMessage("Erreur load fleche droite: %s", IMG_GetError());
        return 0;
    }
    m->flecheDroite = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);

    return 1;
}

static void libererAssetsMenuPerso(MenuPersoAssets *m) {
    SDL_DestroyTexture(m->background);
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(m->persos[i]);
    }
    SDL_DestroyTexture(m->flecheGauche);
    SDL_DestroyTexture(m->flecheDroite);
}

void afficherMenuPersonnage(Game *game, AudioAssets *audio) {
    MenuPersoAssets assets = {0};
    if (!chargerAssetsMenuPerso(game->jeu.renderer, &assets)) {
        return;
    }

    int selection = 0;
    int quitter = 0;
    SDL_Event e;

    while (!quitter) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quitter = 1;
            } else if (e.type == SDL_KEYDOWN) {
                gererClavierMenuPersonnage(&e, &selection, &quitter, audio);
            }
        }

        renderMenuPersonnage(game, &assets, selection);
    }

    libererAssetsMenuPerso(&assets);
}

static void gererClavierMenuPersonnage(SDL_Event *e, int *selection, int *quitter, AudioAssets *audio) {
    switch (e->key.keysym.sym) {
        case SDLK_ESCAPE:
            *quitter = 1;
            break;
        case SDLK_LEFT:
            *selection = (*selection - 1 + 3) % 3;
            jouerSon(audio->buttonChange);
            break;
        case SDLK_RIGHT:
            *selection = (*selection + 1) % 3;
            jouerSon(audio->buttonChange);
            break;
        case SDLK_RETURN:
            // Sélectionner le personnage
            jouerSon(audio->buttonSelect);
            const char* paths[3] = {
                "./assets/character/spritesheet_marcus.png",
                "./assets/character/spritesheet_sarah.png",
                "./assets/character/spritesheet_alex.png"
            };
            cheminSpriteSheetPersonnage = paths[*selection];
            logMessage("Personnage sélectionné: %s", cheminSpriteSheetPersonnage);
            *quitter = 1;
            break;
    }
}

static void renderMenuPersonnage(Game *game, MenuPersoAssets *assets, int selection) {
    SDL_RenderClear(game->jeu.renderer);
    
    // Afficher le background
    SDL_RenderCopy(game->jeu.renderer, assets->background, NULL, NULL);

    // Dimensions et espacement
    int persoWidth = 300;
    int persoHeight = 300;
    int spacing = 70;

    // Calcul des positions pour centrer verticalement et horizontalement
    int centerY = (game->jeu.hauteurEcran / 2) - (persoHeight / 2); // Centre vertical
    int centerX = game->jeu.largeurEcran / 2; // Centre horizontal

    // Afficher les personnages avec le système de carousel
    for (int i = -1; i <= 1; i++) {
        int index = (selection + i + 3) % 3;
        int x = centerX + i * (persoWidth + spacing) - (persoWidth / 2);
        SDL_Rect dst = {
            .x = x,
            .y = centerY,
            .w = persoWidth,
            .h = persoHeight
        };

        SDL_SetTextureColorMod(assets->persos[index], 
                             (i == 0) ? 255 : 150,
                             (i == 0) ? 255 : 150,
                             (i == 0) ? 255 : 150);
        SDL_RenderCopy(game->jeu.renderer, assets->persos[index], NULL, &dst);
    }

    // Repositionner les flèches en fonction du nouveau centre
    int flecheWidth = 150;
    int flecheHeight = 150;
    
    SDL_Rect flecheGauche = {
        .x = centerX - (persoWidth + spacing + flecheWidth),
        .y = centerY + (persoHeight - flecheHeight) / 2, // Aligner verticalement avec les personnages
        .w = flecheWidth,
        .h = flecheHeight
    };
    
    SDL_Rect flecheDroite = {
        .x = centerX + persoWidth + spacing,
        .y = centerY + (persoHeight - flecheHeight) / 2, // Aligner verticalement avec les personnages
        .w = flecheWidth,
        .h = flecheHeight
    };

    SDL_RenderCopy(game->jeu.renderer, assets->flecheGauche, NULL, &flecheGauche);
    SDL_RenderCopy(game->jeu.renderer, assets->flecheDroite, NULL, &flecheDroite);

    SDL_RenderPresent(game->jeu.renderer);
}

// ...existing code...

void gererClicBoutonBoutique();
