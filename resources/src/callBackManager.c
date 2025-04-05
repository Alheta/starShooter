#include <stdio.h>
#include <stdlib.h>
#include "callBackManager.h"

#include "entity.h"

// Массив для хранения зарегистрированных коллбэков
static CallbackEntry callbacks[MAX_CALLBACKS];
static int callback_count = 0;

// Функция для добавления коллбэков
void AddCallback(CallbackType type, void (*callback)(void*), void* filter_data) {
    if (callback_count < MAX_CALLBACKS) {
        callbacks[callback_count].type = type;
        callbacks[callback_count].callback = callback;
        callbacks[callback_count].filter_data = filter_data;
        callback_count++;
    }
}

void CallCallbacks(CallbackType type, void* data) {
    Entity* entity = (Entity*)data;

    for (int i = 0; i < callback_count; i++) {
        if (callbacks[i].type != type) {
            continue;
        }

        switch (callbacks[i].type)
        {
            case POST_ENEMY_DEATH:
                Entity* entity = (Entity*)data;
                if (callbacks[i].filter_data != NULL && entity->variant != (int)(intptr_t)callbacks[i].filter_data)
                {
                    continue;
                }
                break;

            default:
                break;
        }

        printf("Result!\n");
        callbacks[i].callback(data);
    }
}

void RegisterAllCallbacks() {

    //Коллбэк для отслеживания смерти врагов
    AddCallback(POST_ENEMY_DEATH, OnExplosiveAsteroidKill, (void* )ENEMY_EXPLOSIVE_ASTEROID);
}