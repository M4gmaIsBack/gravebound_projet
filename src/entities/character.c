#include "character.h"
#include "../logs/logging.h"
#include "../UI/menu_personnage.h" 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "zombies.h"

Personnage personnage;

Personnage charger_personnage(SDL_Renderer* renderer, char *save) {

    personnage.direction = -1;  // Initialisation à une valeur impossible pour vérifier si le chargement a réussi

    if (!renderer) {
        logMessage("ERREUR: renderer est nul dans chargerPersonnage");
        return personnage;
    }

    logMessage("Chargement du personnage avec la sprite-sheet: %s", cheminSpriteSheetPersonnage);
    SDL_Surface* surface = IMG_Load(cheminSpriteSheetPersonnage); 
    if (!surface) {
        logMessage("Erreur de chargement de la sprite-sheet du personnage: %s", IMG_GetError());
        return personnage;
    }

    personnage.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!personnage.texture) {
        logMessage("Erreur de création de la texture du personnage: %s", SDL_GetError());
        return personnage;
    }

    personnage.frameWidth = 32;
    personnage.frameHeight = 32;
    personnage.currentFrame = 0;
    personnage.totalFrames = 3;

    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/personnages.txt", save);
    FILE *fichier = fopen(filepath, "r");
    if (fichier == NULL || (fscanf(fichier, "%d %d %d %d %d %d %f %f %d %d %f\n", &personnage.x, &personnage.y, &personnage.vie_max, &personnage.vie_actuelle, &personnage.force_attaque, &personnage.invincibilite, &personnage.vitesse, &personnage.vitesse_max, &personnage.direction, &personnage.moving, &personnage.defense) != 11)) {
        personnage.direction = 0;
        personnage.moving = 0;
        personnage.x = 0; 
        personnage.y = 0; 
        personnage.vie_max = 100;
        personnage.vie_actuelle = 100;
        personnage.force_attaque = 25;
        personnage.invincibilite = 0;
        personnage.vitesse = 5;
        personnage.vitesse_max = 5;
        personnage.defense = 0;
    }

    return personnage;
}

void mettreAJourPersonnage(const Uint8* state) {
    personnage.moving = 0;

    // Détermine la direction selon les touches pressées (ZQSD)
    if (state[SDL_SCANCODE_W]) {
        personnage.direction = 3;
        personnage.y -= personnage.vitesse;
        personnage.moving = 1;
    } else if (state[SDL_SCANCODE_S]) {
        personnage.direction = 0;
        personnage.y += personnage.vitesse;
        personnage.moving = 1;
    } else if (state[SDL_SCANCODE_A]) {
        personnage.direction = 1;
        personnage.x -= personnage.vitesse;
        personnage.moving = 1;
    } else if (state[SDL_SCANCODE_D]) { 
        personnage.direction = 2;
        personnage.x += personnage.vitesse;
        personnage.moving = 1;
    }

    // Met à jour la frame uniquement si le personnage bouge
    if (personnage.moving) {
        personnage.currentFrame = (personnage.currentFrame + 1) % personnage.totalFrames;
    } else {
        personnage.currentFrame = 1;
    }
    
    // Mise à jour du compteur d'invincibilité
    if (personnage.invincibilite > 0) {
        personnage.invincibilite--;
    }

    // Attaque avec la touche ESPACE
    if (state[SDL_SCANCODE_SPACE]) {
        attaquerZombies();
    }
}

void dessinerPersonnage(SDL_Renderer* renderer, int x, int y) {
    if (!personnage.texture) {
        logMessage("Erreur: la texture du personnage n'est pas chargée.");
        return;
    }

    SDL_Rect srcRect = {
        personnage.currentFrame * personnage.frameWidth,
        personnage.direction * personnage.frameHeight,
        personnage.frameWidth, personnage.frameHeight
    };

    SDL_Rect destRect = { x, y, personnage.frameWidth, personnage.frameHeight };

    SDL_RenderCopy(renderer, personnage.texture, &srcRect, &destRect);
}

void obtenirPositionPersonnage(int* x, int* y) {
    *x = personnage.x;
    *y = personnage.y;
}

int obtenirViePersonnage() {
    return personnage.vie_actuelle;
}

void subirDegatsPersonnage(int degats) {
    if (personnage.invincibilite <= 0) {
        personnage.vie_actuelle -= degats;
        personnage.invincibilite = 60; // ~1 seconde d'invincibilité
        logMessage("Le personnage subit %d dégâts! Vie restante: %d", degats, personnage.vie_actuelle);
    }
}

int estPersonnageVivant() {
    return personnage.vie_actuelle > 0;
}

void attaquerZombies() {
    for (int i = 0; i < nombre_zombies; i++) {
        int dx = zombies[i]->x - personnage.x;
        int dy = zombies[i]->y - personnage.y;
        float distance = sqrt(dx * dx + dy * dy);
    }
}

void fermerPersonnage() {
    if (personnage.texture) {
        SDL_DestroyTexture(personnage.texture);
        personnage.texture = NULL;
        logMessage("Texture du personnage fermée.");
    }
}

void dessinerBarreDeVie(SDL_Renderer* renderer, int x, int y, int largeur, int hauteur, int vie_actuelle, int vie_max) {
    SDL_Rect fond = { x, y, largeur, hauteur };
    SDL_Rect barre = { x, y, (int)((double)largeur * vie_actuelle / vie_max), hauteur };

    // Dessiner le fond de la barre de vie (rouge)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &fond);

    // Dessiner la barre de vie (verte)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &barre);
}

void enregistrer_personnage(char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/config/personnages.txt", save);
    FILE *fichier = fopen(filepath, "w");
    if (fichier == NULL) {
        logMessage("Erreur ouverture fichier personnage.txt");
        return;
    }
    fprintf(fichier, "%d %d %d %d %d %d %f %f %d %d %f\n", personnage.x, personnage.y, personnage.vie_max, personnage.vie_actuelle, personnage.force_attaque, personnage.invincibilite, personnage.vitesse, personnage.vitesse_max, personnage.direction, personnage.moving, personnage.defense);
    fclose(fichier);
}