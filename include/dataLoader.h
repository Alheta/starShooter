#ifndef DATALOADER_H
#define DATALOADER_H

#include "raylib.h"
#include "entity.h"
#include "sfxManager.h"

typedef struct {
    EntityData entities[100];
    SoundData sounds[100]; // Максимум 100 звуков
} GameData;

extern GameData gameData;

char* ReadJSONFile(const char* filename);
void LoadDataFromJson(const char *filename);

#endif // DATALOADER_H