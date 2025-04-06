#include "raylib.h"
#include "sfxManager.h"
#include "dataLoader.h"
#include <stdlib.h>

void SFXPlay(SoundID sound, float volume, float pitch, float pan) {  // Changed name
    int variantCount = gameData.sounds[sound].variantCount;
    if (variantCount > 0) {
        int randomIndex = rand() % variantCount;
        StopSound(gameData.sounds[sound].variants[randomIndex]);
        PlaySound(gameData.sounds[sound].variants[randomIndex]);
        SetSoundVolume(gameData.sounds[sound].variants[randomIndex], volume);
        SetSoundPitch(gameData.sounds[sound].variants[randomIndex], pitch);
    }
}
