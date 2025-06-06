#include <stdio.h>
#include <stdlib.h>
#include "callBackManager.h"

#include "entity.h"
#include "player.h"
#include "game.h"
#include "UI.h"

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

    for (int i = 0; i < callback_count; i++) {
        if (callbacks[i].type != type) {
            continue;
        }

        bool shouldSkip = false;
        switch (callbacks[i].type)
        {
            case POST_ENEMY_DEATH:
                Entity* entity = (Entity*)data;
                if (entity->data.variant != (int)(intptr_t)callbacks[i].filter_data)
                {
                    shouldSkip = true;
                    continue;
                }
                break;
            case POST_POWER_UP_PICKUP: {
                ShootFlags passedFlag = (ShootFlags)(intptr_t)data;
                ShootFlags expectedFlag = (ShootFlags)(intptr_t)callbacks[i].filter_data;

                if ((passedFlag & expectedFlag) == 0) {
                    shouldSkip = true;
                    continue;
                }
                break;
            }
            case POST_POWER_UP_REMOVE: {
                ShootFlags passedFlag = (ShootFlags)(intptr_t)data;
                ShootFlags expectedFlag = (ShootFlags)(intptr_t)callbacks[i].filter_data;

                if ((passedFlag & expectedFlag) == 0) {
                    shouldSkip = true;
                    continue;
                }
                break;
            }
            case POST_BUTTON_CLICK: {
                ButtonType passedType = (ButtonType)(intptr_t)data;
                ButtonType excpectedType = (ButtonType)(intptr_t)callbacks[i].filter_data;

                if (passedType != excpectedType) {
                    shouldSkip = true;
                    continue;
                }
                break;
            }
            default:
                break;
        }
        if (!shouldSkip) {
            callbacks[i].callback(data);
        }
    }
}

void RegisterAllCallbacks() {

    //Коллбэк для отслеживания смерти врагов
    AddCallback(POST_ENEMY_DEATH, OnAsteroidKill, (void* ) 0);
    AddCallback(POST_ENEMY_DEATH, OnExplosiveAsteroidKill, (void* ) 1);
    AddCallback(POST_ENEMY_DEATH, OnPoweredAsteroidKill, (void* ) 2);

    AddCallback(POST_POWER_UP_PICKUP, OnSpeedBoostApply, (void*)(intptr_t)SHOOT_SPEED);
    AddCallback(POST_POWER_UP_PICKUP, OnTripleApply, (void* )(intptr_t)SHOOT_TRIPLE_SHOT);
    AddCallback(POST_POWER_UP_PICKUP, OnCoolingApply, (void*)(intptr_t)SHOOT_COOLING);
    AddCallback(POST_POWER_UP_PICKUP, OnHomingApply, (void* )(intptr_t)SHOOT_HOMING);

    AddCallback(POST_POWER_UP_REMOVE, OnSpeedBoostRemove, (void*)(intptr_t)SHOOT_SPEED);
    AddCallback(POST_POWER_UP_REMOVE, OnTripleRemove, (void* )(intptr_t)SHOOT_TRIPLE_SHOT);
    AddCallback(POST_POWER_UP_REMOVE, OnCoolingRemove, (void*)(intptr_t)SHOOT_COOLING);
    AddCallback(POST_POWER_UP_REMOVE, OnHomingRemove, (void* )(intptr_t)SHOOT_HOMING);

    AddCallback(POST_BUTTON_CLICK, OnStartClick, (void*)(intptr_t)BUTTON_START);
    AddCallback(POST_BUTTON_CLICK, OnExitClick, (void*)(intptr_t)BUTTON_EXIT);
    AddCallback(POST_BUTTON_CLICK, OnMenuClick, (void*)(intptr_t)BUTTON_MENU);
    AddCallback(POST_BUTTON_CLICK, OnRetryClick, (void*)(intptr_t)BUTTON_RETRY);
}