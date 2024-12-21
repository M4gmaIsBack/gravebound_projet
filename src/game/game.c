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

    logMessage("Jeu initialisé avec succès");
    return 1;
}


// Boucle principale de jeu
// Gere les evenements, les entrees et le rendu
void bouclePrincipale(Game *game) {
    SDL_Event event;

    logMessage("Début de la boucle principale");
    while (game->running) {
        logMessage("Attente des événements SDL");

        while (SDL_PollEvent(&event)) {
            logMessage("Événement détecté : %d", event.type);

            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                logMessage("Quitter la boucle principale");
                game->running = 0;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
                logMessage("Basculer en mode plein écran");
                toggleFullscreen(&game->jeu);
            }

            if (event.type == SDL_KEYDOWN) {
                logMessage("Gestion du déplacement clavier");
                gererDeplacementClavier(&event, &game->jeu);
            }

            if (event.type == SDL_CONTROLLERAXISMOTION) {
                logMessage("Gestion du déplacement manette");
                gererDeplacementCarte(&event, &game->jeu);
            }
        }

        logMessage("Mise à jour du rendu");
        majRendu(&game->jeu);

        SDL_Delay(16); // 60 FPS
    }
    logMessage("Fin de la boucle principale");
}



void nettoyerRessources(Game *game) {
    fermerManette();
    fermerGraphique(&game->jeu);
}