#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>
#include "../logs/logging.h"

typedef struct {
    Mix_Chunk *buttonChange;
    Mix_Chunk *buttonSelect;
} AudioAssets;

void configurerVolume(AudioAssets *audio, int volume);


int initAudio(AudioAssets *audio);

void jouerSon(Mix_Chunk *sound);

void libererAudio(AudioAssets *audio);

#endif
