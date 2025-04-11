#include "powerUp.h"
#include "entity.h"
#include "constants.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void OnSpeedBoostApply()
{
    Entity* player = GetPlayer();
    player->data.speed=BOOST_SPEED;
    player->data.toPlayer.maxFireDelay=BOOST_FIREDELAY;
    printf("INFO: Speed Boost Applied!\n");
}

void OnSpeedBoostRemove()
{
    Entity* player = GetPlayer();
    player->data.speed=NORMAL_SPEED;
    player->data.toPlayer.maxFireDelay=NORMAL_FIREDELAY;
    printf("INFO: Speed Boost Removed!\n");
}

static Texture2D powerUpIcons[ShootFlags_COUNT];

void LoadPowerUpIcons() {
    powerUpIcons[SHOOT_TRIPLE_SHOT] = LoadTexture("resources/gfx/power ups/triple_shot_00.png");
    powerUpIcons[SHOOT_HOMING] = LoadTexture("resources/gfx/power ups/homing_00.png");
    powerUpIcons[SHOOT_COOLING] = LoadTexture("resources/gfx/power ups/cooling_00.png");
    powerUpIcons[SHOOT_SPEED] = LoadTexture("resources/gfx/power ups/speed_boost_00.png");
}

Texture2D GetIconForPowerUp(ShootFlags type) {
    if (type >= 0 && type < ShootFlags_COUNT)
        return powerUpIcons[type];
}


//Функции-пустышки, т.к. вся логика уже прописана с назначением флага

void OnTripleApply()
{
    printf("INFO: Triple Shot Applied!\n");
}

void OnTripleRemove()
{
    printf("INFO: Triple Shot Removed!\n");
}

void OnHomingApply()
{
    printf("INFO: Homing Applied!\n");
}

void OnHomingRemove()
{
    printf("INFO: Homing Removed!\n");
}

void OnCoolingApply()
{
    printf("INFO: Cooling Applied!\n");
}

void OnCoolingRemove()
{
    printf("INFO: Cooling Removed!\n");
}