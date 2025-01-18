#include "game/game.h"
#include "logs/logging.h"
#include "UI/menu.h"
#include "audio/audio.h"

int initialiserJeuComplet(Game *game, AudioAssets *audio) {
    if (!initJeu(game)) {
        logMessage("Echec initialisation du jeu");
        return 0;
    }

    if (!initAudio(audio)) {
        logMessage("Erreur initialisation audio");
        nettoyerRessources(game);
        return 0;
    }

    configurerVolume(audio, 10);

    return 1;
}

void libererJeuComplet(Game *game, AudioAssets *audio) {
    libererAudio(audio);
    nettoyerRessources(game);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Game game;
    AudioAssets audio;

    if (!initialiserJeuComplet(&game, &audio)) {
        return 1;
    }

    afficherMenuPrincipal(&game, &audio);

    libererJeuComplet(&game, &audio);

    return 0;
}
