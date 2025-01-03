#include "game.h"
#include "../ui/graphique.h"
#include "../game/time.h"
#include "../controller/controller.h"
#include "../logs/logging.h"

// Initialise le jeu
// Retourne 1 si c'est good, 0 en cas d'echec
int initJeu(Game *game) {
    game->running = 1;

    if (!initGraphique(&game->jeu)) {
        logMessage("Erreur initialisation graphique");
        return 0;
    }



    if (!init_time(&game->jeu.countdown, (time){12, 0, 0})) {
        logMessage("Erreur initialisation temps");
        return 0;
    }

    printf("Temps initialisé à %d:%d:%d\n", game->jeu.countdown.hour, game->jeu.countdown.minute, game->jeu.countdown.second);

    logMessage("Jeu initialisé avec succès");
    return 1;
}

// Boucle principale de jeu
void bouclePrincipale(Game *game) {
    SDL_Event event;

    logMessage("Début de la boucle principale");
    while (game->running) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                game->running = 0;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
                toggleFullscreen(&game->jeu);
            }

            // Gestion des deplacements
            if (event.type == SDL_KEYDOWN) {
                gererDeplacementClavier(&event, &game->jeu);
            }
            if (event.type == SDL_CONTROLLERAXISMOTION) {
                gererDeplacementCarte(&event, &game->jeu);
            }
        }

        static Uint32 lastUpdateTime = 0;
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastUpdateTime + 1000) {
            update_time(&game->jeu.countdown);
            display_time(&game->jeu.countdown);
            lastUpdateTime = currentTime;
        }

        majRendu(&game->jeu);

        SDL_Delay(16);
    }
    logMessage("Fin de la boucle principale");
}

// Nettoie les ressources du jeu
void nettoyerRessources(Game *game) {
    fermerManette();
    fermerGraphique(&game->jeu);
}
