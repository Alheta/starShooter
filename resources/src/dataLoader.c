#include "cJSON.h"
#include "dataLoader.h"

#include "raylib.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

GameData gameData;

const char* enemyIdNames[] = {

}

const char* soundIdNames[] = {
    "SOUND_SHOOT",
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

void LoadDataFromJson(const char *filename, GameData *gameData) {
    char *fileContent = ReadJSONFile(filename);  // Читаем файл
    cJSON *root = cJSON_Parse(fileContent);
    if (!root) {
        printf("Error parsing JSON!\n");
        return;
    }



    // Загрузка данных сущностей
    if (strcmp(filename, "config/entities.json") == 0) {
        cJSON *entities = cJSON_GetObjectItem(root, "entities");

        // Загрузка врагов
        cJSON *enemies = cJSON_GetObjectItem(entities, "enemies");
        for (int i = 0; i < cJSON_GetArraySize(enemies); i++) {
            cJSON *enemyJson = cJSON_GetArrayItem(enemies, i);
            Entity enemy = {0};
            enemy.type = ENEMY;
            enemy.health = cJSON_GetObjectItem(enemyJson, "health")->valueint;
            enemy.speed = cJSON_GetObjectItem(enemyJson, "speed")->valuedouble;
            cJSON *colorJson = cJSON_GetObjectItem(enemyJson, "color");
            enemy.color.r = cJSON_GetArrayItem(colorJson, 0)->valueint;
            enemy.color.g = cJSON_GetArrayItem(colorJson, 1)->valueint;
            enemy.color.b = cJSON_GetArrayItem(colorJson, 2)->valueint;
            strcpy(enemy.spritePath, cJSON_GetObjectItem(enemyJson, "sprite")->valuestring);
            enemy.enemy.score = cJSON_GetObjectItem(enemyJson, "score")->valueint;
            enemy.variant = cJSON_GetObjectItem(enemyJson, "variant")->valueint;
            enemy.sprite = LoadTexture(enemy.spritePath);

            gameData->enemies[i] = enemy;
        }

        // Загрузка пуль
        cJSON *bullets = cJSON_GetObjectItem(entities, "bullets");
        for (int i = 0; i < cJSON_GetArraySize(bullets); i++) {
            cJSON *bulletJson = cJSON_GetArrayItem(bullets, i);
            Entity bullet = {0};
            bullet.type = BULLET;
            bullet.bullet.damage = cJSON_GetObjectItem(bulletJson, "damage")->valueint;
            bullet.speed = cJSON_GetObjectItem(bulletJson, "speed")->valuedouble;
            strcpy(bullet.spritePath, cJSON_GetObjectItem(bulletJson, "sprite")->valuestring);
            bullet.variant = cJSON_GetObjectItem(bulletJson, "variant")->valueint;
            bullet.sprite = LoadTexture(bullet.spritePath);

            gameData->bullets[i] = bullet;
        }

        // Загрузка усилений
        cJSON *powerups = cJSON_GetObjectItem(entities, "powerups");
        for (int i = 0; i < cJSON_GetArraySize(powerups); i++) {
            cJSON *powerUpJson = cJSON_GetArrayItem(powerups, i);
            Entity powerUp = {0};
            powerUp.type = POWER_UP;
            powerUp.powerUp.duration = cJSON_GetObjectItem(powerUpJson, "duration")->valuedouble;
            strcpy(powerUp.spritePath, cJSON_GetObjectItem(powerUpJson, "sprite")->valuestring);
            gameData->powerups[i] = powerUp;
        }
    }

    // Загрузка звуков
    if (strcmp(filename, "config/sounds.json") == 0) {
        cJSON *sounds = cJSON_GetObjectItem(root, "sounds");
        for (int i = 0; i < SOUND_COUNT_ENUM; i++) {
            cJSON *soundItem = cJSON_GetObjectItem(sounds, soundIdNames[i]);
            if (soundItem) {
                int soundCount = cJSON_GetArraySize(soundItem);
                gameData->sounds[i].variantCount = soundCount;

                for (int j = 0; j < soundCount; j++) {
                    const char *soundPath = cJSON_GetArrayItem(soundItem, j)->valuestring;
                    // Загружаем звук
                    gameData->sounds[i].variants[j]  = LoadSound(soundPath);

                    // Проверяем, был ли звук загружен успешно
                    if (gameData->sounds[i].variants[j].frameCount == 0) {
                        printf("Error loading sound: %s\n", soundPath);
                    }
                }
            }
        }
    }


    free(fileContent);
}