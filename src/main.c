#include "game/game.h"
#include "logs/logging.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Game game;

    if (!initJeu(&game)) {
        logMessage("Echec initialisation du jeu");
        return 1;
    }

    bouclePrincipale(&game);
    nettoyerRessources(&game);

    return 0;
}