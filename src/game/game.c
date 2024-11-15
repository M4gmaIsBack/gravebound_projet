// Gestion du cycle de vie principal du jeu
#include "game.h"
#include "../ui/graphique.h"
#include "../controller/controller.h"
#include "../logs/logging.h"

// Initialise le jeu
// Retourne 1 si succes, 0 en cas d'echec
int initJeu(Game *game) {
    game->running = 1;

    if (!initGraphique(&game->jeu)) {
        logMessage("Erreur initialisation graphique");
        return 0;
    }

    // Initialisation du peripherique de la manette
    if (!initManette()) {
        logMessage("Erreur initialisation manette");
        return 0;
    }

    logMessage("Jeu initialisé avec succès");
    return 1;
}

// Boucle principale de jeu
// Gere les evenements, les entrees et le rendu
void bouclePrincipale(Game *game) {
    SDL_Event event;
    int running = game->running;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
               (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }

            // Gestion du déplacement via la manette
            if (event.type == SDL_CONTROLLERAXISMOTION) {
                gererDeplacementCarte(&event, &game->jeu);
            }

            // Gestion du déplacement clavier
            if (event.type == SDL_KEYDOWN) {
                gererDeplacementClavier(&event, &game->jeu);
            }
        }

        majRendu(&game->jeu);

        SDL_Delay(16); // Limitation à 60 FPS
    }
}



void nettoyerRessources(Game *game) {
    fermerManette();
    fermerGraphique(&game->jeu);
}