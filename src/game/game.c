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
void bouclePrincipale(Game *game, char *save, Personnage *personnage) {
    SDL_Event event;

    logMessage("Début de la boucle principale");

    int skill_selected = 0;

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
                gererDeplacementClavier(&event, &game->jeu, personnage);
                skill_selected = skill_selection(&event, game, personnage, skill_selected);      
                skill_activation(&event, game, personnage, skill_selected);
            }

            if (event.type == SDL_CONTROLLERAXISMOTION) {
                gererDeplacementCarte(&event, &game->jeu, personnage);
            }
        }

        static Uint32 lastUpdateTimeCD = 0;
        Uint32 currentTimeCD = SDL_GetTicks();
        if (currentTimeCD > lastUpdateTimeCD + 1000) {
            update_time(&game->jeu.countdown);
            cooldown_skills(personnage);
            display_time(&game->jeu.countdown);
            lastUpdateTimeCD = currentTimeCD;
        }


        static Uint32 lastUpdateTimeZ = 0;
        Uint32 currentTimeZ = SDL_GetTicks();
        if (currentTimeZ > lastUpdateTimeZ + 5000) {
            if (game->jeu.countdown.time > 20 || game->jeu.countdown.time < 8) {
                spawn_zombies((game->jeu.largeurEcran / 2) - game->jeu.carteX, (game->jeu.hauteurEcran / 2) - game->jeu.carteY , 1000);
            }
            lastUpdateTimeZ = currentTimeZ;
        }

        if (personnage->vitesse > personnage->vitesse_max) {
            personnage->vitesse -= 0.005;
        } else if (personnage->vitesse < personnage->vitesse_max) {
            personnage->vitesse += 0.005;
        }

        majRendu(&game->jeu);

        enregistrer_coordonnees(&game->jeu, save);

        SDL_Delay(16);
    }

    enregistrer_progression(game, save, personnage);
    logMessage("Fin de la boucle principale");


}

// Nettoie les ressources du jeu
void nettoyerRessources(Game *game) {
    logMessage("Nettoyage des ressources du jeu");

    fermerGraphique(&game->jeu);

    logMessage("Ressources nettoyées");
}

void enregistrer_progression(Game *game, char *save, Personnage *personnage) {
    enregistrer_time(&game->jeu.countdown, save);
    enregistrer_zombies(save);
    enregistrer_personnage(save);
    enregistrer_skills(personnage, save);
}

Personnage charger_progression(Game *game, char *save) {
    init_carte(&game->jeu, save);
    init_time(&game->jeu.countdown, (time){12, 0, 0, 0, 0, 20}, save);
    charger_zombies(save);
    charger_coordonnees(&game->jeu, save);
    Personnage personnage = charger_personnage(game->jeu.renderer, save);
    charger_skills(&personnage, save);

    return personnage;
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
    Personnage personnage = charger_progression(game, save);

    if (personnage.direction == -1) {
        logMessage("Erreur chargement/initialisation du personnage");
        return;
    }


    game->running = 1;
    bouclePrincipale(game, save, &personnage);
}

void charger_coordonnees(Jeu *jeu, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/coord.txt", save);
    FILE *fichier = fopen(filepath, "r");
    if (fichier == NULL || (fscanf(fichier, "%d %d", &jeu->carteX, &jeu->carteY) != 2)) {
        jeu->carteX = -(jeu->map.taille * LARGEUR_CASE / 2) + (jeu->largeurEcran / 2);
        jeu->carteY = -(jeu->map.taille * HAUTEUR_CASE / 2) + (jeu->hauteurEcran / 2);
    }
    if (fichier != NULL) {
        fclose(fichier);
    }
}

void enregistrer_coordonnees(Jeu *jeu, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/coord.txt", save);
    FILE *fichier = fopen(filepath, "w");
    if (fichier) {
        fprintf(fichier, "%d %d\n", jeu->carteX, jeu->carteY);
        fclose(fichier);
    } else {
        logMessage("Erreur ouverture fichier coord.txt");
    }
}