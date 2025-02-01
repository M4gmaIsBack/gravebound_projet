#include "game.h"
#include "../ui/graphique.h"
#include "../game/time.h"
#include "../controller/controller.h"
#include "../logs/logging.h"
#include "../entities/character.h"
#include "../entities/attack.h"
#include <SDL2/SDL.h>
#include <sys/stat.h>
#include <errno.h>
#include "../entities/zombies.h"
#include <cjson/cJSON.h>
#include "../map/map.h"

int vague = 0;

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

    init_attacks(game->jeu.renderer);

    logMessage("Jeu initialisé avec succès");
    return 1;
}

// Boucle principale de jeu
void bouclePrincipale(Game *game, char *save, Personnage *personnage) {
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    logMessage("Début de la boucle principale");

    int skill_selected = 0;

    while (game->running) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                game->running = 0;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                skip_day(&game->jeu.countdown);
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
                toggleFullscreen(&game->jeu);
            }

            // Gestion des deplacements
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                skill_selected = skill_selection(&event, game, personnage, skill_selected);      
                skill_activation(&event, game, personnage, skill_selected);
            }

            if (event.type == SDL_CONTROLLERAXISMOTION) {
                gererDeplacementCarte(&event, &game->jeu, personnage);
            }

            if (event.type == SDL_CONTROLLERDEVICEADDED) {
                game->using_controller = 1;
            }
            if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
                game->using_controller = 0;
            }

            // Gestion des attaques à la souris
            if (!game->using_controller) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                
                int playerScreenX = game->jeu.largeurEcran / 2;
                int playerScreenY = game->jeu.hauteurEcran / 2;
                
                handle_mouse_attack(mouseX, mouseY, playerScreenX, playerScreenY);
            }
        }

        gererDeplacementClavier(keystate, &game->jeu, personnage);

        static Uint32 lastUpdateTimeCD = 0;
        Uint32 currentTimeCD = SDL_GetTicks();
        if (currentTimeCD > lastUpdateTimeCD + 1000) {
            update_time(&game->jeu.countdown);
            cooldown_skills(personnage);
            display_time(&game->jeu.countdown);

            if (game->jeu.countdown.elapsed_time + game->jeu.countdown.OFFSET * 60 > (vague * 24 + game->jeu.countdown.OFFSET) * 60) {
                vague++;
            }

            if(game->jeu.countdown.hour == 0 && game->jeu.countdown.minute == 0 && game->jeu.countdown.second == 0 && vague > 0) {
                final_wave();
            }

            lastUpdateTimeCD = currentTimeCD;
        }


        static Uint32 lastUpdateTimeZ = 0;
        Uint32 currentTimeZ = SDL_GetTicks();
        if (currentTimeZ > lastUpdateTimeZ + config.zombies.spawn_delay[vague] * 1000) { //5000
            if (game->jeu.countdown.time > 20 || game->jeu.countdown.time < 8) {
                spawn_zombies((game->jeu.largeurEcran / 2) - game->jeu.carteX, (game->jeu.hauteurEcran / 2) - game->jeu.carteY , config.zombies.spawn_radius[vague]); //1000
            }
            lastUpdateTimeZ = currentTimeZ;
        }


        if (personnage->vitesse > personnage->vitesse_max) {
            personnage->vitesse -= 0.005;
        } else if (personnage->vitesse < personnage->vitesse_max) {
            personnage->vitesse += 0.005;
        }

        int joueurCarteX = game->jeu.largeurEcran / 2 - game->jeu.carteX;
        int joueurCarteY = game->jeu.hauteurEcran / 2 - game->jeu.carteY;

        update_attacks(&game->jeu, joueurCarteX, joueurCarteY, game->using_controller);

        majRendu(&game->jeu, personnage);
        
        enregistrer_coordonnees(&game->jeu, save);

        SDL_Delay(16);
    }

    enregistrer_progression(game, save, personnage);
    logMessage("Fin de la boucle principale");
}

// Nettoie les ressources du jeu
void nettoyerRessources(Game *game) {
    logMessage("Nettoyage des ressources du jeu");

    cleanup_attacks();

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
    init_time(&game->jeu.countdown, save);
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
    sprintf(filepath, "saves/%s/config", save);
    mkdir(filepath);
    sprintf(filepath, "saves/%s/source", save);
    mkdir(filepath);

    if (!charger_config(save)) {
        logMessage("Erreur chargement config");
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
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/coord.json", save);
    FILE *fichier = fopen(filepath, "r");
    if (fichier == NULL) {
        jeu->carteX = -(jeu->map.taille * LARGEUR_CASE / 2) + (jeu->largeurEcran / 2);
        jeu->carteY = -(jeu->map.taille * HAUTEUR_CASE / 2) + (jeu->hauteurEcran / 2);
        return;
    }

    char buffer[1024];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, fichier);
    fclose(fichier);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        jeu->carteX = -(jeu->map.taille * LARGEUR_CASE / 2) + (jeu->largeurEcran / 2);
        jeu->carteY = -(jeu->map.taille * HAUTEUR_CASE / 2) + (jeu->hauteurEcran / 2);
        return;
    }

    cJSON *carteX = cJSON_GetObjectItem(json, "carteX");
    cJSON *carteY = cJSON_GetObjectItem(json, "carteY");

    if (carteX && carteY) {
        jeu->carteX = carteX->valueint;
        jeu->carteY = carteY->valueint;
    } else {
        jeu->carteX = -(jeu->map.taille * LARGEUR_CASE / 2) + (jeu->largeurEcran / 2);
        jeu->carteY = -(jeu->map.taille * HAUTEUR_CASE / 2) + (jeu->hauteurEcran / 2);
    }

    cJSON_Delete(json);
}

void enregistrer_coordonnees(Jeu *jeu, char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/coord.json", save);
    FILE *fichier = fopen(filepath, "w");
    if (fichier) {
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "carteX", jeu->carteX);
        cJSON_AddNumberToObject(json, "carteY", jeu->carteY);

        char *json_string = cJSON_Print(json);
        fprintf(fichier, "%s\n", json_string);

        free(json_string);
        cJSON_Delete(json);
        fclose(fichier);
    } else {
        logMessage("Erreur ouverture fichier coord.json");
    }
}