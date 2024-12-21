#include "game/game.h"
#include "logs/logging.h"
#include "UI/menu.h"
#include "audio/audio.h"

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    Game game;

    // Initialisation du jeu
    if (!initJeu(&game)) {
        logMessage("Echec initialisation du jeu");
        return 1;
    }

    // Initialisation de l'audio
    // Initialisation de l'audio
    AudioAssets audio;
    if (!initAudio(&audio)) {
        logMessage("Erreur initialisation audio");
        nettoyerRessources(&game); // Libère les ressources graphiques si l'audio échoue
        return 1;
    }

    // Configurer le volume (entre 0 et MIX_MAX_VOLUME, par exemple 64 pour un volume moyen)
    configurerVolume(&audio, 10);


    // Affichage du menu principal
    afficherMenuPrincipal(&game, &audio);

    // Lancer le jeu si le joueur le sélectionne dans le menu
    if (game.running) {
        bouclePrincipale(&game);
    }

    // Libération des ressources
    libererAudio(&audio);
    nettoyerRessources(&game);

    return 0;
}
