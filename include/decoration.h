#ifndef DECORATION_H
#define DECORATION_H

#include "powerUp.h"
#include "raylib.h"

typedef struct DecoSpawner
{
    int starSpawnDelay;
    int planetSpawnDelay;
    int giantSpawnDelay;
} DecoSpawner;

DecoSpawner* GetDecorationSpawner();
void InitDecorationSpawner();

void SpawnDecoration(Vector2 pos, int variant);
void DecorationSpawner();

int GetRandomGiantPosition();

#endif //DECORATION_H