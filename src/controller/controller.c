#include "controller.h"
#include "../logs/logging.h"
#include <SDL2/SDL_gamecontroller.h>

SDL_GameController *controller = NULL;

// Initialise la connexion avec la manette
// Retourne 1 si OK, 0 en cas d'échec
int initManette() {
    // Vérification de la présence d'une manette
    if (SDL_NumJoysticks() < 1) {
        logMessage("Aucune manette détectée");
        return 0;
    }

    // Tentative d'ouverture de la manette
    controller = SDL_GameControllerOpen(0);
    if (!controller) {
        logMessage("Erreur lors de l'ouverture de la manette : %s", SDL_GetError());
        return 0;
    }

    logMessage("Manette détectée et initialisée");
    return 1;
}

// Ferme la connexion avec la manette
void fermerManette() {
    if (controller) {
        SDL_GameControllerClose(controller);
        controller = NULL;
        logMessage("Manette déconnectée");
    }
}

// Gestion des déplacements via clavier
void gererDeplacementClavier(SDL_Event *event, Jeu *jeu) {
    switch (event->key.keysym.sym) {
        case SDLK_z: // Haut
<<<<<<< HEAD
            jeu->carteY -= JOYSTICK_VITESSE_MAX;
            logMessage("Déplacement haut (clavier)");
            break;
        case SDLK_s: // Bas
            jeu->carteY += JOYSTICK_VITESSE_MAX;
            logMessage("Déplacement bas (clavier)");
            break;
        case SDLK_q: // Gauche
            jeu->carteX -= JOYSTICK_VITESSE_MAX;
            logMessage("Déplacement gauche (clavier)");
            break;
        case SDLK_d: // Droite
            jeu->carteX += JOYSTICK_VITESSE_MAX;
            logMessage("Déplacement droite (clavier)");
=======
            jeu->carteY += JOYSTICK_VITESSE_MAX;
            logMessage("Deplacement haut (clavier)");
            break;
        case SDLK_s: // Bas
            jeu->carteY -= JOYSTICK_VITESSE_MAX;
            logMessage("Deplacement bas (clavier)");
            break;
        case SDLK_q: // Gauche
            jeu->carteX += JOYSTICK_VITESSE_MAX;
            logMessage("Deplacement gauche (clavier)");
            break;
        case SDLK_d: // Droite
            jeu->carteX -= JOYSTICK_VITESSE_MAX;
            logMessage("Deplacement droite (clavier)");
>>>>>>> 268c361a94de3d5f8b603f75e45dca57d2633fa5
            break;
    }
}

// Gestion des déplacements via la manette
void gererDeplacementCarte(SDL_Event *event, Jeu *jeu) {
    if (event->type == SDL_CONTROLLERAXISMOTION) {
        // Récupération des valeurs horizontal et vertical
        int valeurX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
        int valeurY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

        // Déplacement sur l'axe horizontal
        if (abs(valeurX) > JOYSTICK_ZONE_MORTE) {
            float deplacement = (float)valeurX / 32768.0f * JOYSTICK_VITESSE_MAX;
            jeu->carteX += (int)deplacement;
            logMessage("Déplacement horizontal (manette) : %d", (int)deplacement);
        }

        // Déplacement sur l'axe vertical
        if (abs(valeurY) > JOYSTICK_ZONE_MORTE) {
            float deplacement = (float)valeurY / 32768.0f * JOYSTICK_VITESSE_MAX;
            jeu->carteY += (int)deplacement;
            logMessage("Déplacement vertical (manette) : %d", (int)deplacement);
        }
    }
}
