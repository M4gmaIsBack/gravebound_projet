#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>
#include "../logs/logging.h"

// Structure pour gérer les sons
typedef struct {
    Mix_Chunk *buttonChange; // Son pour le changement de bouton
    Mix_Chunk *buttonSelect; // Son pour la sélection d'un bouton
} AudioAssets;

void configurerVolume(AudioAssets *audio, int volume);


// Initialisation de l'audio
int initAudio(AudioAssets *audio);

// Lecture d'un son
void jouerSon(Mix_Chunk *sound);

// Libération des ressources audio
void libererAudio(AudioAssets *audio);

#endif
