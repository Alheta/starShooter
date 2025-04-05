#ifndef DATALOADER_H
#define DATALOADER_H

#include "raylib.h"
#include "entity.h"
#include "sfxManager.h"

typedef struct {
    Entity enemies[100];  // Максимум 100 врагов
    Entity bullets[100];  // Максимум 100 пуль
    Entity powerups[100]; // Максимум 100 усилений
    SoundGroupData sounds[100]; // Максимум 100 звуков
} GameData;

extern GameData gameData;

char* ReadJSONFile(const char* filename);
void LoadDataFromJson(const char *filename, GameData *gameData);

#endif // DATALOADER_H