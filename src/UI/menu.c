#include "menu.h"
#include "../audio/audio.h"
#include "../logs/logging.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Charger les assets du menu
static int chargerAssetsMenu(SDL_Renderer *r, MenuAssets *m) {
    // Charger le background
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

    // Charger les boutons
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

    logMessage("Assets menu chargés avec succès");
    return 1;
}

// Libérer les assets du menu
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
    logMessage("Assets menu libérés");
}

// Rendu du menu
static void renderMenu(SDL_Renderer *r, MenuAssets *m, int sel, int largeurEcran, int hauteurEcran) {
    SDL_RenderClear(r);

    // Afficher le background
    SDL_RenderCopy(r, m->background, NULL, NULL);

    // Dimensions des boutons
    int buttonWidth = 290;
    int buttonHeight = 80;

    int boutiqueWidth = 150; 
    int boutiqueHeight = 170;

    // Positions des boutons
    SDL_Rect dsts[4];
    dsts[0] = (SDL_Rect){(largeurEcran - buttonWidth) / 2, hauteurEcran - 180, buttonWidth, buttonHeight}; // Play (centré en bas)
    dsts[1] = (SDL_Rect){(largeurEcran / 2) - buttonWidth - 200, hauteurEcran - 150, buttonWidth, buttonHeight}; // Settings (plus à gauche)
    dsts[2] = (SDL_Rect){(largeurEcran / 2) + 200, hauteurEcran - 150, buttonWidth, buttonHeight}; // Credit (plus à droite)
    dsts[3] = (SDL_Rect){largeurEcran - boutiqueWidth - 30, 30, boutiqueWidth, boutiqueHeight}; // Boutique (en haut à droite)

    // Rendu des boutons
    for (int i = 0; i < 4; i++) {
        SDL_SetTextureColorMod(m->buttons[i], (i == sel) ? 255 : 200, (i == sel) ? 255 : 200, (i == sel) ? 255 : 200);
        SDL_RenderCopy(r, m->buttons[i], NULL, &dsts[i]);
    }

    SDL_RenderPresent(r);
}


// Gérer les événements du menu
static void gererEvenementsMenu(SDL_Event *e, int *sel, int *quitter, AudioAssets *audio) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) {
            *quitter = 1; // Quitter l'application
        }

        // Gestion des événements clavier
        else if (e->type == SDL_KEYDOWN) {
            switch (e->key.keysym.sym) {
                case SDLK_ESCAPE:
                    *quitter = 1; // Quitter avec Echap
                    break;
                case SDLK_RIGHT: // Flèche gauche clavier
                    if (*sel == 0) *sel = 2; // Play vers Credit
                    else if (*sel == 1) *sel = 0; // Settings vers Play
                    else if (*sel == 2) *sel = 1; // Credit vers Settings
                    jouerSon(audio->buttonChange); // Son pour changement
                    logMessage("Navigation gauche clavier : %d", *sel);
                    break;
                case SDLK_LEFT: // Flèche droite clavier
                    if (*sel == 0) *sel = 1; // Play vers Settings
                    else if (*sel == 1) *sel = 2; // Settings vers Credit
                    else if (*sel == 2) *sel = 0; // Credit vers Play
                    jouerSon(audio->buttonChange); // Son pour changement
                    logMessage("Navigation droite clavier : %d", *sel);
                    break;
                case SDLK_UP: // Flèche haut clavier
                    *sel = 3; // Aller à Boutique
                    jouerSon(audio->buttonChange); // Son pour changement
                    logMessage("Navigation haut clavier : %d", *sel);
                    break;
                case SDLK_DOWN: // Flèche bas clavier
                    if (*sel == 3) *sel = 0; // Boutique vers Play
                    jouerSon(audio->buttonChange); // Son pour changement
                    logMessage("Navigation bas clavier : %d", *sel);
                    break;
                case SDLK_RETURN: // Valider l'option
                    *quitter = 2;
                    jouerSon(audio->buttonSelect); // Son pour validation
                    logMessage("Option validée clavier : %d", *sel);
                    break;
            }
        }

        // Gestion des événements manette
        else if (e->type == SDL_CONTROLLERBUTTONDOWN) {
            logMessage("Bouton de la manette appuyé : %d", e->cbutton.button);

            switch (e->cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: // Flèche gauche manette
                    if (*sel == 0) *sel = 2; // Play vers Credit
                    else if (*sel == 1) *sel = 0; // Settings vers Play
                    else if (*sel == 2) *sel = 1; // Credit vers Settings
                    jouerSon(audio->buttonChange); // Son pour changement
                    logMessage("Navigation gauche manette : %d", *sel);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT: // Flèche droite manette
                    if (*sel == 0) *sel = 1; // Play vers Settings
                    else if (*sel == 1) *sel = 2; // Settings vers Credit
                    else if (*sel == 2) *sel = 0; // Credit vers Play
                    jouerSon(audio->buttonChange); // Son pour changement
                    logMessage("Navigation droite manette : %d", *sel);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP: // Flèche haut manette
                    *sel = 3; // Aller à Boutique
                    jouerSon(audio->buttonChange); // Son pour changement
                    logMessage("Navigation haut manette : %d", *sel);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN: // Flèche bas manette
                    if (*sel == 3) *sel = 0; // Boutique vers Play
                    jouerSon(audio->buttonChange); // Son pour changement
                    logMessage("Navigation bas manette : %d", *sel);
                    break;
                case SDL_CONTROLLER_BUTTON_A: // Bouton A pour valider
                    *quitter = 2;
                    jouerSon(audio->buttonSelect); // Son pour validation
                    logMessage("Option validée manette : %d", *sel);
                    break;
            }
        }
    }
}
// Afficher le menu principal
void afficherMenuPrincipal(Game *game, AudioAssets *audio) {
    int quitter = 0;
    int sel = 0;
    SDL_Event e;

    MenuAssets menuAssets;

    if (!chargerAssetsMenu(game->jeu.renderer, &menuAssets)) {
        logMessage("Erreur chargement des assets du menu");
        return;
    }

    logMessage("Affichage du menu principal");

    while (!quitter) {
        gererEvenementsMenu(&e, &sel, &quitter, audio);
        renderMenu(game->jeu.renderer, &menuAssets, sel, game->jeu.largeurEcran, game->jeu.hauteurEcran);
    }

    if (quitter == 2 && sel == 0) {
        logMessage("Lancement du jeu depuis le menu principal");
        game->running = 1;
        if (!chargerCarte(&game->jeu)) {
            logMessage("Erreur lors du chargement de la carte !");
            game->running = 0;
        }
    } else {
        logMessage("Option sélectionnée : %d", sel);
        game->running = 0;
    }

    libererAssetsMenu(&menuAssets);
    logMessage("Fin de la fonction afficherMenuPrincipal");
}
