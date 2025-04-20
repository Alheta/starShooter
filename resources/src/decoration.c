#include "decoration.h"
#include "game.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>


static DecoSpawner decoSpawner;

DecoSpawner* GetDecorationSpawner()
{
    return &decoSpawner;
}

void InitDecorationSpawner()
{
    DecoSpawner* decoSpawner = GetDecorationSpawner();
    decoSpawner->starSpawnDelay=0;
    decoSpawner->planetSpawnDelay=0;
    decoSpawner->giantSpawnDelay=0;
}

void SpawnDecoration(Vector2 pos, int variant) {
    Entity* ent = SpawnEntity(ENTITY_DECORATION, variant, pos, (Vector2){0, 1});

    ent->zIndex = -100;

    float depth = (80 + rand() % 120);

    Color baseColor = (Color){10, 0, 20, 255};

    Color decorationColor = (Color){120, 100, 180, 255};
    float darkenFactor = (255 - depth) / 180.0f;

    decorationColor.r = (unsigned char)(decorationColor.r * darkenFactor);
    decorationColor.g = (unsigned char)(decorationColor.g * darkenFactor);
    decorationColor.b = (unsigned char)(decorationColor.b * darkenFactor);

    ent->defaultColor.r = (unsigned char)((decorationColor.r + baseColor.r) / 2);
    ent->defaultColor.g = (unsigned char)((decorationColor.g + baseColor.g) / 2);
    ent->defaultColor.b = (unsigned char)((decorationColor.b + baseColor.b) / 2);
    ent->defaultColor.a = 255;

    ent->size.x = depth/160;
    ent->size.y = depth/160;

    ent->data.speed *= depth/(750*(variant+0.5));

}

void DecorationSpawner()
{
    DecoSpawner* decoSpawner = GetDecorationSpawner();
    if (decoSpawner->starSpawnDelay <= 0)
    {
        if (rand() % 100 < 30)
        {
            Vector2 pos = { rand() % SCREEN_WIDTH, -600 };
            SpawnDecoration(pos, 0);
            decoSpawner->starSpawnDelay = 4;
        }
    }

    if (decoSpawner->planetSpawnDelay <= 0)
    {
        if (rand() % 1500 < 250)
        {
            Vector2 pos = { rand() % SCREEN_WIDTH, -600 };
            SpawnDecoration(pos, 1);
            decoSpawner->planetSpawnDelay = 360;
        }
    }

    if (decoSpawner->planetSpawnDelay <= 0)
    {
        if (rand() % 2000 < 300)
        {
            Vector2 pos = {GetRandomGiantPosition(), -600 };
            SpawnDecoration(pos, 2);
            decoSpawner->planetSpawnDelay = 1200;
        }
    }

    if (decoSpawner->starSpawnDelay>0) decoSpawner->starSpawnDelay--;
    if (decoSpawner->planetSpawnDelay >0) decoSpawner->planetSpawnDelay--;
    if (decoSpawner->giantSpawnDelay >0) decoSpawner->giantSpawnDelay--;
}

int GetRandomGiantPosition()
{
    int zone = GetRandomValue(0, 2); // 0 = слева, 1 = справа, 2 = центр
    switch (zone)
    {
        case 0: // слева
            return GetRandomValue(-100, 0);

        case 1: // справа
            return GetRandomValue(SCREEN_WIDTH, SCREEN_WIDTH + 100);

        case 2: // центр
            return GetRandomValue(SCREEN_WIDTH / 2 - 200, SCREEN_WIDTH / 2 + 200);
    }

    return SCREEN_WIDTH / 2;
}
