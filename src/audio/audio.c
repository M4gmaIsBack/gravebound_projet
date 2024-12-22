#include "audio.h"

#include "audio.h"

void configurerVolume(AudioAssets *audio, int volume) {
    if (volume < 0) volume = 0;
    if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;

    // Appliquer le volume à chaque effet sonore
    Mix_VolumeChunk(audio->buttonChange, volume);
    Mix_VolumeChunk(audio->buttonSelect, volume);

    logMessage("Volume réglé à %d", volume);
}


int initAudio(AudioAssets *audio) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        logMessage("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        return 0;
    }

    // Charger les sons
    audio->buttonChange = Mix_LoadWAV("./assets/song/button_change.mp3");
    if (!audio->buttonChange) {
        logMessage("Erreur chargement son button_change : %s", Mix_GetError());
        return 0;
    }

    audio->buttonSelect = Mix_LoadWAV("./assets/song/button_select.mp3");
    if (!audio->buttonSelect) {
        logMessage("Erreur chargement son button_select : %s", Mix_GetError());
        return 0;
    }

    logMessage("Audio initialisé avec succès");
    return 1;
}

void jouerSon(Mix_Chunk *sound) {
    if (sound) {
        Mix_PlayChannel(-1, sound, 0);
    } else {
        logMessage("Son non trouvé");
    }
}

void libererAudio(AudioAssets *audio) {
    if (audio->buttonChange) {
        Mix_FreeChunk(audio->buttonChange);
        audio->buttonChange = NULL;
    }
    if (audio->buttonSelect) {
        Mix_FreeChunk(audio->buttonSelect);
        audio->buttonSelect = NULL;
    }
    Mix_CloseAudio();
    logMessage("Ressources audio libérées");
}
