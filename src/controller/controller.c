#include "controller.h"
#include "../logs/logging.h"
#include <SDL2/SDL_gamecontroller.h>

SDL_GameController *controller = NULL;

// Initialise la connexion avec la manette
int initManette() {
    if (SDL_NumJoysticks() < 1) {
        logMessage("Aucune manette détectée");
        return 0;
    }

    controller = SDL_GameControllerOpen(0);
    if (!controller) {
        logMessage("Erreur lors de l'ouverture de la manette : %s", SDL_GetError());
        return 0;
    }

    logMessage("Manette détectée et initialisée");
    return 1;
}

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
            break;
    }
}

// Gestion des deplacements via la manette
void gererDeplacementCarte(SDL_Event *event, Jeu *jeu) {
    if (event->type == SDL_CONTROLLERAXISMOTION) {
        int valeurX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
        int valeurY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

        if (abs(valeurX) > JOYSTICK_ZONE_MORTE) {
            float deplacement = (float)valeurX / 32768.0f * JOYSTICK_VITESSE_MAX;
            jeu->carteX += (int)deplacement;
            logMessage("Déplacement horizontal (manette) : %d", (int)deplacement);
        }

        if (abs(valeurY) > JOYSTICK_ZONE_MORTE) {
            float deplacement = (float)valeurY / 32768.0f * JOYSTICK_VITESSE_MAX;
            jeu->carteY += (int)deplacement;
            logMessage("Déplacement vertical (manette) : %d", (int)deplacement);
        }
    }
}
