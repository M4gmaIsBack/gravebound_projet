#include "game.h"
#include "../ui/graphique.h"
#include "../game/time.h"
#include "../controller/controller.h"
#include "../logs/logging.h"
#include "../entities/character.h"
#include <SDL2/SDL.h>
#include "../entities/zombies.h"

// Initialise le jeu
// Retourne 1 si c'est good, 0 en cas d'echec
int initJeu(Game *game) {
    logMessage("Initialisation du jeu");
    game->running = 1;

    if (!initGraphique(&game->jeu)) {
        logMessage("Erreur initialisation graphique");
        return 0;
    }

    // Vérifier que le renderer n'est pas nul
    if (!game->jeu.renderer) {
        logMessage("Renderer est nul après initGraphique");
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

        static Uint32 lastUpdateTimeCD = 0;
        Uint32 currentTimeCD = SDL_GetTicks();
        if (currentTimeCD > lastUpdateTimeCD + 1000) {
            update_time(&game->jeu.countdown);
            display_time(&game->jeu.countdown);
            lastUpdateTimeCD = currentTimeCD;
        }

        float time = fmod(game->jeu.countdown.elapsed_time / 60.0, 24.0);


        static Uint32 lastUpdateTimeZ = 0;
        Uint32 currentTimeZ = SDL_GetTicks();
        if (currentTimeZ > lastUpdateTimeZ + 5000) {
            if (time > 20 || time < 8) {
                printf("spawn %d, %d\n", game->jeu.carteX, game->jeu.carteY);
                spawn_zombies((game->jeu.largeurEcran / 2) - game->jeu.carteX, (game->jeu.hauteurEcran / 2) - game->jeu.carteY , 1000);
            }
            lastUpdateTimeZ = currentTimeZ;
        }

        majRendu(&game->jeu);

        SDL_Delay(16);
    }
    logMessage("Fin de la boucle principale");
}

// Nettoie les ressources du jeu
void nettoyerRessources(Game *game) {
    logMessage("Nettoyage des ressources du jeu");
    fermerManette();
    fermerGraphique(&game->jeu);
}

void lancerJeu(Game *game) {
    logMessage("Lancement du jeu");

    // Initialiser le personnage
    if (!chargerPersonnage(game->jeu.renderer)) {
        logMessage("Erreur chargement personnage, abandon.");
        return;
    }

    logMessage("Personnage chargé avec succès");

    // Boucle principale du jeu
    game->running = 1;
    bouclePrincipale(game);

    logMessage("Libération des ressources du personnage");
    // Libérer les ressources du personnage
    fermerPersonnage();
}
