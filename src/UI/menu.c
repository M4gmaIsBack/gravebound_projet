#include "menu.h"
#include "menu_personnage.h" 
#include "../audio/audio.h"
#include "../logs/logging.h"
#include "../entities/character.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static void gererClavierMenu(SDL_Event *e, int *sel, int *quitter, Game *game, AudioAssets *audio);

static int chargerAssetsMenu(SDL_Renderer *r, MenuAssets *m) {
    SDL_Surface *surface = IMG_Load("./assets/menu/background.jpg");
    if (!surface) {
        logMessage("Erreur load background: %s", IMG_GetError());
        return 0;
    }
    m->background = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);
    if (!m->background) {
        logMessage("Erreur création texture background: %s", SDL_GetError());
        return 0;
    }

    const char* paths[4] = {
        "./assets/menu/play.png",
        "./assets/menu/settings.png",
        "./assets/menu/credit.png",
        "./assets/menu/boutique.png"
    };

    for (int i = 0; i < 4; i++) {
        surface = IMG_Load(paths[i]);
        if (!surface) {
            logMessage("Erreur load bouton %s: %s", paths[i], IMG_GetError());
            return 0;
        }
        m->buttons[i] = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);
        if (!m->buttons[i]) {
            logMessage("Erreur création texture bouton %s: %s", paths[i], SDL_GetError());
            return 0;
        }
    }

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

static void libererAssetsMenu(MenuAssets *m) {
    if (m->background) {
        SDL_DestroyTexture(m->background);
        m->background = NULL;
    }
    for (int i = 0; i < 4; i++) {
        if (m->buttons[i]) {
            SDL_DestroyTexture(m->buttons[i]);
            m->buttons[i] = NULL;
        }
    }
    if (m->flecheGauche) {
        SDL_DestroyTexture(m->flecheGauche);
        m->flecheGauche = NULL;
    }
    if (m->flecheDroite) {
        SDL_DestroyTexture(m->flecheDroite);
        m->flecheDroite = NULL;
    }
}

static void renderMenu(SDL_Renderer *r, MenuAssets *m, int sel, int largeurEcran, int hauteurEcran) {
    SDL_RenderClear(r);

    SDL_RenderCopy(r, m->background, NULL, NULL);

    int buttonWidth = 300;
    int buttonHeight = 300;
    int spacing = 70;

    int flecheWidth = 150;
    int flecheHeight = 150;

    SDL_Rect dstFlecheGauche = {
        .x = largeurEcran / 2 - buttonWidth * 2 - spacing - flecheWidth,
        .y = hauteurEcran - 250,
        .w = flecheWidth,
        .h = flecheHeight
    };
    SDL_Rect dstFlecheDroite = {
        .x = largeurEcran / 2 + buttonWidth * 2 + spacing,
        .y = hauteurEcran - 250,
        .w = flecheWidth,
        .h = flecheHeight
    };

    for (int i = -1; i <= 1; i++) {
        int index = (sel + i + 4) % 4;
        int x = largeurEcran / 2 + i * (buttonWidth + spacing) - buttonWidth / 2;
        SDL_Rect dst = {.x = x, .y = hauteurEcran - 300, .w = buttonWidth, .h = buttonHeight};

        SDL_SetTextureColorMod(m->buttons[index], (i == 0) ? 255 : 150, (i == 0) ? 255 : 150, (i == 0) ? 255 : 150);
        SDL_RenderCopy(r, m->buttons[index], NULL, &dst);
    }

    SDL_RenderCopy(r, m->flecheGauche, NULL, &dstFlecheGauche);
    SDL_RenderCopy(r, m->flecheDroite, NULL, &dstFlecheDroite);

    SDL_RenderPresent(r);
}

// Gestion carousel bouton
static void gererEvenementsMenu(SDL_Event *e, int *sel, int *quitter, Game *game, AudioAssets *audio) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) {
            *quitter = 1;
        } else if (e->type == SDL_KEYDOWN) {
            gererClavierMenu(e, sel, quitter, game, audio);
        }
    }
}

static void gererClavierMenu(SDL_Event *e, int *sel, int *quitter, Game *game, AudioAssets *audio) {
    switch (e->key.keysym.sym) {
        case SDLK_ESCAPE:
            *quitter = 1;
            break;
        case SDLK_RIGHT:
            *sel = (*sel + 1) % 4;
            jouerSon(audio->buttonChange);
            break;
        case SDLK_LEFT:
            *sel = (*sel - 1 + 4) % 4;
            jouerSon(audio->buttonChange);
            break;
        case SDLK_RETURN:
            jouerSon(audio->buttonSelect);
            if (*sel == 0) { 
                *quitter = 2; 
            } else if (*sel == 3) { 
                afficherMenuPersonnage(game, audio);
            } else {
                *quitter = 2;
            }
            break;
    }
}

// Affichage du menu principal
void afficherMenuPrincipal(Game *game, AudioAssets *audio) {
    logMessage("Affichage du menu principal");
    int quitter = 0;
    int sel = 0;
    SDL_Event e;

    MenuAssets menuAssets = {0};

    if (!chargerAssetsMenu(game->jeu.renderer, &menuAssets)) {
        logMessage("Erreur chargement des assets du menu");
        return;
    }

    while (!quitter) {
        gererEvenementsMenu(&e, &sel, &quitter, game, audio);
        renderMenu(game->jeu.renderer, &menuAssets, sel, game->jeu.largeurEcran, game->jeu.hauteurEcran);
    }

    libererAssetsMenu(&menuAssets);

    if (quitter == 2) {
        char* save = "partie_test";
        lancerJeu(game, save);  
    }
}
