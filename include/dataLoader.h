#ifndef DATALOADER_H
#define DATALOADER_H

#include "raylib.h"
#include "debug.h"
#include "entity.h"
#include "sfxManager.h"
#include "UI.h"

typedef struct {
    EntityData entities[100];
    SoundData sounds[100]; // Максимум 100 звуков
    Button buttons[5];

    DebugFlags debugFlags;
} GameData;

extern GameData gameData;

char* ReadJSONFile(const char* filename);
void LoadDataFromJson(const char *filename);
void LoadConstantSprites();
void LoadButtonData();

#endif // DATALOADER_H