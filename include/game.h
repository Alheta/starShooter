#ifndef GAME_H
#define GAME_H

#include "entity.h"
#include "raylib.h"

typedef enum ZoneType {
    GAMEPLAY_ZONE,
    DELETION_ZONE,
    SPAWNER_ZONE
} ZoneType;

typedef struct Zone {
    float baseWidth;
    float baseHeight;

    Vector2 position;
    float scale;

    Rectangle rec;

    ZoneType type;
} Zone;

typedef struct Spawner {
    int spawnDelay;
    int spawnPoolSize;
} Spawner;

//Работа с Зонами
void AddZone(float baseWidth, float baseHeight, ZoneType type, float scale);
void ZoneUpdate();
void DrawZones();
Zone* GetZoneByType(ZoneType zone);
bool IsInsideZone(Rectangle rec, ZoneType type);
Vector2 GetRandomPositionInZone(ZoneType type);

//Работа с Энтити
Entity** GetGameEntities();
Entity* SpawnEntity(EntityType type, int variant, Vector2 position, Vector2 velocity);
void* KillEntity(Entity* entity);
Entity* GetClosestEntity(Vector2 position, float radius, EntityType type);

//Спавнер
int GetRandomEnemyFromPool();
void InitSpawner();
Spawner* GetSpawner();
void SpawnerUpdate();

//Другое
float DegreeToRadian(float degree);
void  OnButtonClick(void* data);

#endif // GAME_H