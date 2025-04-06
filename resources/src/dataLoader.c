#include "cJSON.h"
#include "dataLoader.h"
#include "raylib.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

GameData gameData;

const char* soundIdNames[] = {
    "SOUND_SHOOT",
    "PLAYER_DAMAGE",
    "ENEMY_DEATH",
    "ENEMY_DEATH_BIG"
};


char* ReadJSONFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* content = (char*)malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
}

void LoadDataFromJson(const char *filename) {

    char *fileContent = ReadJSONFile(filename);  // Читаем файл
    cJSON *root = cJSON_Parse(fileContent);
    if (!root) {
        printf("Error parsing JSON!\n");
        return;
    }

    // Загрузка данных сущностей
    if (strcmp(filename, "config/entities.json") == 0) {
        cJSON *entities = cJSON_GetObjectItem(root, "entities");

        for (int i = 0; i < cJSON_GetArraySize(entities); i++) {
            cJSON* entity = cJSON_GetArrayItem(entities, i);
            EntityData entityData = {0};

            // Определяем тип сущности
            const char* typeStr = cJSON_GetObjectItem(entity, "type")->valuestring;
            if (strcmp(typeStr, "ENEMY") == 0) {
                entityData.type = ENTITY_ENEMY;
            } else if (strcmp(typeStr, "BULLET") == 0) {
                entityData.type = ENTITY_BULLET;
            } else if (strcmp(typeStr, "POWER_UP") == 0) {
                entityData.type = ENTITY_POWERUP;
            } else if (strcmp(typeStr, "PARTICLE") == 0) {
                entityData.type = ENTITY_PARTICLE;
            }


            entityData.variant = cJSON_GetObjectItem(entity, "variant")->valueint;

            entityData.maxHealth = cJSON_GetObjectItem(entity, "health") ? cJSON_GetObjectItem(entity, "health")->valuedouble : 0;
            entityData.speed = cJSON_GetObjectItem(entity, "speed") ? cJSON_GetObjectItem(entity, "speed")->valuedouble : 1;
            entityData.damage = cJSON_GetObjectItem(entity, "damage") ? cJSON_GetObjectItem(entity, "damage")->valuedouble : 0;
            entityData.collisionRadius = cJSON_GetObjectItem(entity, "collisionRadius") ? cJSON_GetObjectItem(entity, "collisionRadius")->valuedouble : 0;

            cJSON* sprites = cJSON_GetObjectItem(entity, "sprite");
            int spriteCount = cJSON_GetArraySize(sprites);
            for (int i = 0; i < spriteCount; i++) {
                const char *entSprite = cJSON_GetArrayItem(sprites, i)->valuestring;
                entityData.sprite[i] = LoadTexture(entSprite);
            }
            entityData.spriteCount = spriteCount;

            if (entityData.type == ENTITY_ENEMY) {
                entityData.toEnemy.score = cJSON_GetObjectItem(entity, "score") ? cJSON_GetObjectItem(entity, "score")->valueint : 0;
                entityData.toEnemy.weight = cJSON_GetObjectItem(entity, "weight") ? cJSON_GetObjectItem(entity, "weight")->valuedouble : 0;
            } else if (entityData.type == ENTITY_BULLET) {
                entityData.toBullet.homingRadius = cJSON_GetObjectItem(entity, "homingRadius") ? cJSON_GetObjectItem(entity, "homingRadius")->valueint : 0;
            } else if (entityData.type == ENTITY_POWERUP) {
                entityData.toPowerUp.duration = cJSON_GetObjectItem(entity, "duration") ? cJSON_GetObjectItem(entity, "duration")->valuedouble : 0;
            } else if (entityData.type == ENTITY_PARTICLE) {
                entityData.toParticle.timeOut = cJSON_GetObjectItem(entity, "timeout") ? cJSON_GetObjectItem(entity, "timeout")->valuedouble : 0;

            }

            gameData.entities[i] = entityData;
        }
    }

    // Загрузка звуков
    if (strcmp(filename, "config/sounds.json") == 0) {
        cJSON *sounds = cJSON_GetObjectItem(root, "sounds");
        for (int i = 0; i < SOUND_COUNT_ENUM; i++) {
            cJSON *soundItem = cJSON_GetObjectItem(sounds, soundIdNames[i]);
            if (soundItem) {
                int soundCount = cJSON_GetArraySize(soundItem);
                gameData.sounds[i].variantCount = soundCount;

                for (int j = 0; j < soundCount; j++) {
                    const char *soundPath = cJSON_GetArrayItem(soundItem, j)->valuestring;
                    // Загружаем звук
                    gameData.sounds[i].variants[j]  = LoadSound(soundPath);

                    // Проверяем, был ли звук загружен успешно
                    if (gameData.sounds[i].variants[j].frameCount == 0) {
                        printf("Error loading sound: %s\n", soundPath);
                    }
                }
            }
        }
    }

    cJSON_Delete(root);
    free(fileContent);
}