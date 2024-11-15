#include "controller.h"
#include "../logs/logging.h"

SDL_GameController *controller = NULL;

// Initialise la connection avec la manette
// Retourne 1 si ok, 0 en cas d'echec
int initManette() {
    // Verification de la presence d'une manette
    if (SDL_NumJoysticks() < 1) {
        logMessage("Aucune manette detecter");
        return 0;
    }

    // Tentative d'ouverture de la manette
    controller = SDL_GameControllerOpen(0);
    if (!controller) {
        logMessage("Erreur lors de l'ouverture de la manette : %s", SDL_GetError());
        return 0;
    }

    return 1;
}

// Ferme la connexion de la manette
void fermerManette() {
    if (controller) {
        SDL_GameControllerClose(controller);
        controller = NULL;
    }
}

// Gestion du deplacement via clavier
void gererDeplacementClavier(SDL_Event *event, Jeu *jeu) {
    switch (event->key.keysym.sym) {
        case SDLK_z: // Haut
            jeu->carteY -= JOYSTICK_VITESSE_MAX;
            logMessage("Deplacement haut (clavier)");
            break;
        case SDLK_s: // Bas
            jeu->carteY += JOYSTICK_VITESSE_MAX;
            logMessage("Deplacement bas (clavier)");
            break;
        case SDLK_q: // Gauche
            jeu->carteX -= JOYSTICK_VITESSE_MAX;
            logMessage("Deplacement gauche (clavier)");
            break;
        case SDLK_d: // Droite
            jeu->carteX += JOYSTICK_VITESSE_MAX;
            logMessage("Deplacement droite (clavier)");
            break;
    }
}


// Gere le deplacement de la carte
void gererDeplacementCarte(SDL_Event *event, Jeu *jeu) {
    if (event->type == SDL_CONTROLLERAXISMOTION) {
        // Recuperation des valeurs horizontal et vertical
        int valeurX = SDL_GameControllerGetAxis(
            SDL_GameControllerFromInstanceID(event->caxis.which), 
            SDL_CONTROLLER_AXIS_LEFTX
        );
        int valeurY = SDL_GameControllerGetAxis(
            SDL_GameControllerFromInstanceID(event->caxis.which), 
            SDL_CONTROLLER_AXIS_LEFTY
        );

        // Deplacement sur l'axe horizontal
        if (abs(valeurX) > JOYSTICK_ZONE_MORTE) {
            float deplacement = valeurX * JOYSTICK_SENSIBILITE;
            deplacement = fmaxf(-JOYSTICK_VITESSE_MAX, fminf(deplacement, JOYSTICK_VITESSE_MAX));
            jeu->carteX -= deplacement;
        }

        // Deplacement sur l'axe vertical
        if (abs(valeurY) > JOYSTICK_ZONE_MORTE) {
            float deplacement = valeurY * JOYSTICK_SENSIBILITE;
            deplacement = fmaxf(-JOYSTICK_VITESSE_MAX, fminf(deplacement, JOYSTICK_VITESSE_MAX));
            jeu->carteY -= deplacement;
        }
    }
} 