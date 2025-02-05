#include "character.h"
#include "../logs/logging.h"
#include "../UI/menu_personnage.h" 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "zombies.h"
#include "../game/config.h"
#include <cjson/cJSON.h>

Personnage personnage;

Personnage charger_personnage(SDL_Renderer* renderer, char *save) {

    personnage.direction = -1;  // initialisation valeur impossible pour vérifier si le chargement a réussi

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
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/personnages.json", save);
    FILE *fichier = fopen(filepath, "r");
    if (fichier == NULL) {
        personnage.direction = 0;
        personnage.moving = 0;
        personnage.x = 0; 
        personnage.y = 0; 
        personnage.vie_max = config.personnage.vie_max[0];
        personnage.vie_actuelle = config.personnage.vie_max[0];
        personnage.force_attaque = config.personnage.force_attaque_max[0];
        personnage.invincibilite = 0;
        personnage.vitesse = config.personnage.vitesse_max[0];
        personnage.vitesse_max = config.personnage.vitesse_max[0];
        personnage.defense = config.personnage.defense_max[0];
        return personnage;
    }

    char buffer[1024];
    fread(buffer, sizeof(char), sizeof(buffer) - 1, fichier);
    fclose(fichier);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        personnage.direction = 0;
        personnage.moving = 0;
        personnage.x = 0; 
        personnage.y = 0; 
        personnage.vie_max = config.personnage.vie_max[0];
        personnage.vie_actuelle = config.personnage.vie_max[0];
        personnage.force_attaque = config.personnage.force_attaque_max[0];
        personnage.invincibilite = 0;
        personnage.vitesse = config.personnage.vitesse_max[0];
        personnage.vitesse_max = config.personnage.vitesse_max[0];
        personnage.defense = config.personnage.defense_max[0];
        return personnage;
    }

    personnage.x = cJSON_GetObjectItem(json, "x")->valueint;
    personnage.y = cJSON_GetObjectItem(json, "y")->valueint;
    personnage.vie_max = cJSON_GetObjectItem(json, "vie_max")->valueint;
    personnage.vie_actuelle = cJSON_GetObjectItem(json, "vie_actuelle")->valueint;
    personnage.force_attaque = cJSON_GetObjectItem(json, "force_attaque")->valueint;
    personnage.invincibilite = cJSON_GetObjectItem(json, "invincibilite")->valueint;
    personnage.vitesse = cJSON_GetObjectItem(json, "vitesse")->valuedouble;
    personnage.vitesse_max = cJSON_GetObjectItem(json, "vitesse_max")->valuedouble;
    personnage.direction = cJSON_GetObjectItem(json, "direction")->valueint;
    personnage.moving = cJSON_GetObjectItem(json, "moving")->valueint;
    personnage.defense = cJSON_GetObjectItem(json, "defense")->valuedouble;

    cJSON_Delete(json);

    return personnage;
}

void mettreAJourPersonnage(const Uint8* state) {
    personnage.moving = 0;

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

    // frame uniquement si le personnage bouge
    if (personnage.moving) {
        personnage.currentFrame = (personnage.currentFrame + 1) % personnage.totalFrames;
    } else {
        personnage.currentFrame = 1;
    }
    
    // invicibilité
    if (personnage.invincibilite > 0) {
        personnage.invincibilite--;
    }

    // attaque ESPACE (pas toucher c'est annulé)
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

    // barre de vie (rouge)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &fond);

    // barre de vie (verte)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &barre);
}

void enregistrer_personnage(char *save) {
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "./saves/%s/source/personnages.json", save);
    FILE *fichier = fopen(filepath, "w");
    if (fichier == NULL) {
        logMessage("Erreur ouverture fichier personnages.json");
        return;
    }

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "x", personnage.x);
    cJSON_AddNumberToObject(json, "y", personnage.y);
    cJSON_AddNumberToObject(json, "vie_max", personnage.vie_max);
    cJSON_AddNumberToObject(json, "vie_actuelle", personnage.vie_actuelle);
    cJSON_AddNumberToObject(json, "force_attaque", personnage.force_attaque);
    cJSON_AddNumberToObject(json, "invincibilite", personnage.invincibilite);
    cJSON_AddNumberToObject(json, "vitesse", personnage.vitesse);
    cJSON_AddNumberToObject(json, "vitesse_max", personnage.vitesse_max);
    cJSON_AddNumberToObject(json, "direction", personnage.direction);
    cJSON_AddNumberToObject(json, "moving", personnage.moving);
    cJSON_AddNumberToObject(json, "defense", personnage.defense);

    char *json_string = cJSON_Print(json);
    fprintf(fichier, "%s\n", json_string);

    free(json_string);
    cJSON_Delete(json);
    fclose(fichier);
}