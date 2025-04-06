#include "game.h"
#include "gameScreen.h"
#include "player.h"
#include "entity.h"
#include "constants.h"
#include "raymath.h"
#include "dataLoader.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//РАБОТА С ЗОНАМИ//

static Zone zones[3];
static int zoneCount = 0;

void AddZone(float baseWidth, float baseHeight, ZoneType type, float scale) {
    Zone* newZone = &zones[zoneCount];

    newZone->baseWidth = baseWidth * scale;
    newZone->baseHeight = baseHeight * scale;

    newZone->scale = scale;

    newZone->type = type;
    zoneCount++;
}

void ZoneUpdate()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float factor = GetScaleFactor();

    for (int i = 0; i < zoneCount; i++) {
        Zone* zone = &zones[i];


        zone->rec.x = zone->position.x;
        zone->rec.y = zone->position.y;
        zone->rec.width = zone->baseWidth;
        zone->rec.height = zone->baseHeight;

        switch (zone->type) {
            case GAMEPLAY_ZONE:
                zone->position.x = zone->baseWidth / 2 - (zone->rec.width) / 2;
                zone->position.y = zone->baseHeight / 2 - (zone->rec.height) / 2;
                break;
            case DELETION_ZONE:
                zone->position.x = zone->baseWidth / 3.5 - (zone->rec.width) / 2;
                zone->position.y = -1200;
                break;
            case SPAWNER_ZONE:
                zone->position.x = zone->baseWidth / 2 - (zone->rec.width) / 2;
                zone->position.y = -800;
                break;
            default:
                zone->position.x = screenWidth / 2 - (zone->baseWidth * factor) / 2;
                zone->position.y = 20;
                break;
        }
    }
}

void DrawZones() {
    for (int i = 0; i < zoneCount; i++) {
        Zone* zone = &zones[i];
        Color color;

        switch (zone->type) {
            case GAMEPLAY_ZONE: color = GREEN; break;
            case DELETION_ZONE: color = RED; break;
            case SPAWNER_ZONE: color = YELLOW; break;
            default: color = GRAY; break;
        }

        DrawRectangleLines(
            zone->rec.x,
            zone->rec.y,
            zone->rec.width,
            zone->rec.height,
            color
        );
    }
}

Zone* GetZoneByType(ZoneType type) {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i].type == type) {
            return &zones[i];
        }
    }
    return NULL;
}

bool IsInsideZone(Rectangle rec2, ZoneType type) {
    for (int i = 0; i < zoneCount; i++) {
        Zone* zone = &zones[i];
        if (zone->type == type)
        {
            if (CheckCollisionRecs(zone->rec, rec2))
            {
                return true;
            }
        }
    }
    return false;
}

Vector2 GetRandomPositionInZone(ZoneType type) {
    Vector2 randomPosition;
    for (int i = 0; i < zoneCount; i++) {
        Zone* zone = &zones[i];
        if (zone->type == type)
        {
            randomPosition.x = zone->rec.x + GetRandomValue(0, (int)(zone->rec.width - 1));
            randomPosition.y = zone->rec.y + GetRandomValue(0, (int)(zone->rec.height - 1));
        }
    }
    return randomPosition;
}

//РАБОТА С ЭНТИТИ//

Entity* gameEntities[MAX_ENTITIES] = { NULL }; // Массив указателей на сущности

Entity** GetGameEntities()
{
    return gameEntities;
}

Entity* SpawnEntity(EntityType type, int variant, Vector2 position, Vector2 velocity) {
    // Найдем сущность по типу и варианту
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (gameEntities[i] == NULL) {
            Entity* entity = (Entity*)malloc(sizeof(Entity));
            if (!entity) {
                printf("Error: Out of memory!\n");
                return NULL;
            }

            memset(entity, 0, sizeof(Entity));

            // Ищем сущность в gameData по типу и варианту
            for (int j = 0; j < MAX_ENTITIES; j++) {
                if (gameData.entities[j].type == type && gameData.entities[j].variant == variant) {
                    entity->data = gameData.entities[j];  // Копируем данные сущности
                    break;
                }
            }

            entity->color = WHITE;

            entity->randomSpriteIndex = rand() % entity->data.spriteCount;

            entity->position = position;
            entity->velocity = velocity;

            entity->size.x = 1;
            entity->size.y = 1;
            entity->data.sprite[entity->randomSpriteIndex].width = entity->data.sprite[entity->randomSpriteIndex].width * (entity->size.x*SPRITE_SCALE);
            entity->data.sprite[entity->randomSpriteIndex].height = entity->data.sprite[entity->randomSpriteIndex].height * (entity->size.y*SPRITE_SCALE);

            if (IsAsteroid(entity))
            {
                entity->shouldRotate = true;
                entity->spriteRotationSpeed = (rand() % 2001 - 1000) / 1000.f;
            }

            gameEntities[i] = entity;
            return entity;
        }
    }

    printf("Error: Reached the maximum number of entities!\n");
    return NULL;
}

void* KillEntity(Entity* entity)
{
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (gameEntities[i] == entity)
        {
            free(entity);
            gameEntities[i] = NULL;
        }
    }
}

Entity* GetClosestEntity(Vector2 position, float radius, EntityType type)
{
    Entity* nearestEntity = NULL;
    float minDistance = radius;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* entity = gameEntities[i];

        if (entity != NULL && entity->data.type == type) {
            float distance = Vector2Distance(position, entity->position);

            if (distance < minDistance) {
                minDistance = distance;
                nearestEntity = entity;
            }
        }
    }

    return nearestEntity;
}

//СПАВНЕР


static Spawner spawner;

Spawner* GetSpawner() {
    return &spawner;
}
void InitSpawner()
{
    Spawner* spawner = GetSpawner();

    spawner->spawnDelay = 30;
    spawner->spawnPoolSize = 0;
}


// Функция для получения случайного врага из пула, используя вес
int GetRandomEnemyFromPool() {
    Spawner* spawner = GetSpawner();

    // Общий вес всех врагов в пуле
    float totalWeight = 0;
    for (int i = 0; i < 100; i++) { // Проходим по всему массиву сущностей
        if (gameData.entities[i].type == ENTITY_ENEMY) { // Проверка, что это враг
            totalWeight += gameData.entities[i].toEnemy.weight; // Добавляем вес врага
        }
    }

    // Генерация случайного веса для выбора врага
    float randomWeight = (float)rand() / RAND_MAX * totalWeight;

    // Выбор врага на основе случайного веса
    for (int i = 0; i < 100; i++) { // Проходим по массиву сущностей
        if (gameData.entities[i].type == ENTITY_ENEMY) {
            randomWeight -= gameData.entities[i].toEnemy.weight;
            if (randomWeight <= 0) {
                return gameData.entities[i].variant;
            }
        }
    }

    return 0;  // Если не удалось выбрать врага
}

void SpawnerUpdate() {
    Entity* player = GetPlayer();
    Spawner* spawner = GetSpawner();

    if (spawner->spawnDelay == 0) {
        int additionalEnemyCount = player->data.toPlayer.score / 50;

        if (additionalEnemyCount > spawner->spawnPoolSize) {
            // Расширяем пул врагов, если необходимо
            spawner->spawnPoolSize++;

            // Уменьшаем задержку спавна при достижении определенного количества врагов
            if (spawner->spawnDelay > 90) {
                spawner->spawnDelay -= 5;
            }
        }


        // Получаем случайного врага из пула
        int randomEnemy = GetRandomEnemyFromPool();
        float random_angle = 90.0f + (rand() % 91) - 45.0f;
        float angle_rad = DegreeToRadian(random_angle);

        Vector2 velocity;
        velocity.x = cosf(angle_rad) / 4;
        velocity.y = sinf(angle_rad) / 4;

        // Спавним сущность
        SpawnEntity(ENTITY_ENEMY, randomEnemy, GetRandomPositionInZone(SPAWNER_ZONE), velocity);

        spawner->spawnDelay = 25;
    } else {
        spawner->spawnDelay--;
    }
}

//ДРУГОЕ
float DegreeToRadian(float degree) {
    return degree * (3.14159265358979323846f / 180.0f); // Преобразуем в радианы
}
