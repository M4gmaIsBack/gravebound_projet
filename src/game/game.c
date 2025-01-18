#include "game.h"
#include "../ui/graphique.h"
#include "../game/time.h"
#include "../controller/controller.h"
#include "../logs/logging.h"
#include "../entities/character.h"
#include <SDL2/SDL.h>
#include <sys/stat.h>
#include <errno.h>
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

    logMessage("Jeu initialisé avec succès");
    return 1;
}

// Boucle principale de jeu
void bouclePrincipale(Game *game, char *save) {
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


        static Uint32 lastUpdateTimeZ = 0;
        Uint32 currentTimeZ = SDL_GetTicks();
        if (currentTimeZ > lastUpdateTimeZ + 5000) {
            if (game->jeu.countdown.time > 20 || game->jeu.countdown.time < 8) {
                printf("spawn %d, %d\n", game->jeu.carteX, game->jeu.carteY);
                spawn_zombies((game->jeu.largeurEcran / 2) - game->jeu.carteX, (game->jeu.hauteurEcran / 2) - game->jeu.carteY , 1000);
            }
            lastUpdateTimeZ = currentTimeZ;
        }

        majRendu(&game->jeu);

        SDL_Delay(16);
    }

    enregistrer_progression(game, save);
    logMessage("Fin de la boucle principale");


}

// Nettoie les ressources du jeu
void nettoyerRessources(Game *game) {
    logMessage("Nettoyage des ressources du jeu");

    fermerGraphique(&game->jeu);

    logMessage("Ressources nettoyées");
}

void enregistrer_progression(Game *game, char *save) {
    enregistrer_time(&game->jeu.countdown, save);
    enregistrer_zombies(save);
    enregistrer_personnage(game, save);
}

void charger_progression(Game *game, char *save) {
    init_carte(&game->jeu, save);
    init_time(&game->jeu.countdown, (time){12, 0, 0, 0, 0, 17}, save);
    charger_zombies(save);
    charger_personnage(game->jeu.renderer, game, save);
} 

void lancerJeu(Game *game, char *save) {
    logMessage("Lancement du jeu");

    char filepath[100];
    sprintf(filepath, "saves/%s", save);

    mkdir(filepath);
    mkdir(strcat(filepath, "/config"));

    // Vérification que le chemin est un répertoire
    struct stat st;
    if (stat(filepath, &st) == -1 || !S_ISDIR(st.st_mode)) {
        logMessage("Erreur lors de la vérification du répertoire de sauvegarde");
        return;
    }

    logMessage("Répertoire de sauvegarde prêt");
    charger_progression(game, save);

    game->running = 1;
    bouclePrincipale(game, save);
}


