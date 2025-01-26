#include "controller.h"
#include "../logs/logging.h"
#include <SDL2/SDL_gamecontroller.h>

SDL_GameController *controller = NULL;

void skill_activation(SDL_Event *event, Game *game, Personnage *personnage, int slot_selected) {
    switch (event->key.keysym.sym) {
        case SDLK_p:
            char skill_name[10];
            strncpy(skill_name, personnage->skills[slot_selected].name, sizeof(skill_name) - 1);
            skill_name[sizeof(skill_name) - 1] = '\0';

            if (strcmp(skill_name, "Escape") == 0) {

                int x = rand() % game->jeu.hauteurCarte / 2; 
                int y = rand() % game->jeu.largeurCarte / 2;

                if (skill_escape(personnage, x, y, slot_selected)) {
                    game->jeu.carteX = -(x + (game->jeu.largeurEcran / 2));
                    game->jeu.carteY = -(y + (game->jeu.hauteurEcran / 2));
                }

            } else if (strcmp(skill_name, "Teleport") == 0) {

                if(skill_teleport(personnage, slot_selected)) {
                    game->jeu.carteX = -(game->jeu.map.taille * LARGEUR_CASE / 2) + (game->jeu.largeurEcran / 2);
                    game->jeu.carteY = -(game->jeu.map.taille * HAUTEUR_CASE / 2) + (game->jeu.hauteurEcran / 2);
                }
            } else if (strcmp(skill_name, "Knockback") == 0) {

                int centreEcranX = game->jeu.largeurEcran / 2;
                int centreEcranY = game->jeu.hauteurEcran / 2;
            
                int joueurCarteX = centreEcranX - game->jeu.carteX;
                int joueurCarteY = centreEcranY - game->jeu.carteY;

                skill_knockback(personnage, slot_selected, joueurCarteX, joueurCarteY);

            } else if (strcmp(skill_name, "Fire_Ball") == 0) {

                int centreEcranX = game->jeu.largeurEcran / 2;
                int centreEcranY = game->jeu.hauteurEcran / 2;
            
                int joueurCarteX = centreEcranX - game->jeu.carteX;
                int joueurCarteY = centreEcranY - game->jeu.carteY;

                skill_fire_ball(personnage, slot_selected, joueurCarteX, joueurCarteY);
                
            } else if (strcmp(skill_name, "Cage") == 0) {

                int centreEcranX = game->jeu.largeurEcran / 2;
                int centreEcranY = game->jeu.hauteurEcran / 2;
            
                int joueurCarteX = centreEcranX - game->jeu.carteX;
                int joueurCarteY = centreEcranY - game->jeu.carteY;

                skill_cage(personnage, slot_selected, joueurCarteX, joueurCarteY);

            } else if (strcmp(skill_name, "Speed") == 0) {

                skill_light_speed(personnage, slot_selected);

            } else if (strcmp(skill_name, "Berzerk") == 0) {

                skill_berzerk(personnage, slot_selected);

            } else if (strcmp(skill_name, "Kamikaze") == 0) {

                int centreEcranX = game->jeu.largeurEcran / 2;
                int centreEcranY = game->jeu.hauteurEcran / 2;
            
                int joueurCarteX = centreEcranX - game->jeu.carteX;
                int joueurCarteY = centreEcranY - game->jeu.carteY;

                skill_kamikaze(personnage, slot_selected, joueurCarteX, joueurCarteY);

            } else if (strcmp(skill_name, "Heal") == 0) {

                skill_heal(personnage, slot_selected);

            } else if (strcmp(skill_name, "Turtle") == 0) {

                skill_turtle(personnage, slot_selected);

            }

            break;

        case SDLK_o:
            for (int i = 0; i < 10; i++) {
                skill_add(personnage, 1, i);
            }
            break;  
    }
}

int skill_selection(SDL_Event *event, Game *game, Personnage *personnage, int slot_selected) {

    int skills_unlocked = 0;

    for (int i = 0; i < 10; i++) {
        if (strcmp(personnage->skills[i].name, "") != 0) {
            skills_unlocked++;
        }
    }

    switch (event->key.keysym.sym) {
        case SDLK_LEFT:
            if (slot_selected > 0) {
                slot_selected--;
                logMessage("Skill choisi: %s dans slot %d", personnage->skills[slot_selected].name, slot_selected);
            } else {
                slot_selected = skills_unlocked;
                logMessage("Skill choisi: %s dans slot %d", personnage->skills[slot_selected].name, slot_selected);
            }    
            break;
        case SDLK_RIGHT:
            if (slot_selected < skills_unlocked) {
                slot_selected++;
                logMessage("Skill choisi: %s dans slot %d", personnage->skills[slot_selected].name, slot_selected);

            } else {
                slot_selected = 0;
                logMessage("Skill choisi: %s dans slot %d", personnage->skills[slot_selected].name, slot_selected);
            }
            break;
    }

    

    return slot_selected;
}

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
void gererDeplacementClavier(SDL_Event *event, Jeu *jeu, Personnage *personnage) {
    switch (event->key.keysym.sym) {

        case SDLK_z: // Haut
            jeu->carteY += personnage->vitesse;
            // logMessage("Deplacement haut (clavier)");
            break;
        case SDLK_s: // Bas
            jeu->carteY -= personnage->vitesse;
            // logMessage("Deplacement bas (clavier)");
            break;
        case SDLK_q: // Gauche
            jeu->carteX += personnage->vitesse;
            // logMessage("Deplacement gauche (clavier)");
            break;
        case SDLK_d: // Droite
            jeu->carteX -= personnage->vitesse;
            // logMessage("Deplacement droite (clavier)");
            break;
    }
}

// Gestion des deplacements via la manette
void gererDeplacementCarte(SDL_Event *event, Jeu *jeu, Personnage *personnage) {
    if (event->type == SDL_CONTROLLERAXISMOTION) {
        int valeurX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
        int valeurY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

        if (abs(valeurX) > JOYSTICK_ZONE_MORTE) {
            float deplacement = (float)valeurX / 32768.0f * personnage->vitesse;
            jeu->carteX += (int)deplacement;
            logMessage("Déplacement horizontal (manette) : %d", (int)deplacement);
        }

        if (abs(valeurY) > JOYSTICK_ZONE_MORTE) {
            float deplacement = (float)valeurY / 32768.0f * personnage->vitesse;
            jeu->carteY += (int)deplacement;
            logMessage("Déplacement vertical (manette) : %d", (int)deplacement);
        }
    }
}

void gererInputManette(Jeu *jeu, SDL_Event *event) {
    switch(event->type) {
        case SDL_CONTROLLERBUTTONDOWN:
            if (event->cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                toggleFullscreen(jeu);
            }
            break;
        case SDL_WINDOWEVENT:
            switch (event->window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                case SDL_WINDOWEVENT_MINIMIZED:
                case SDL_WINDOWEVENT_RESTORED:
                case SDL_WINDOWEVENT_RESIZED:
                    break;
                default:
                    break;
            }
            break;
    }
}

void toggleFullscreen(Jeu *jeu) {
    Uint32 flags = SDL_GetWindowFlags(jeu->window);
    if (flags & SDL_WINDOW_FULLSCREEN) {
        SDL_SetWindowFullscreen(jeu->window, 0);
        logMessage("Basculé en mode fenêtré");
    } else {
        SDL_SetWindowFullscreen(jeu->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        logMessage("Basculé en mode plein écran");
    }

    // Mettre à jour les dimensions de la fenêtre
    SDL_GetWindowSize(jeu->window, &jeu->largeurEcran, &jeu->hauteurEcran);
    logMessage("Dimensions mises à jour : %dx%d", jeu->largeurEcran, jeu->hauteurEcran);
}