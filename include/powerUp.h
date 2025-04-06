#ifndef POWERUP_H
#define POWERUP_H

typedef enum ShootFlags {
    SHOOT_OVERHEAT = 1 << 0,
    SHOOT_TRIPLE_SHOT =  1 << 1,
    SHOOT_HOMING = 1 << 2,
    SHOOT_COOLING = 1 << 3,
    SHOOT_SPEED = 1 << 4,
    ShootFlags_COUNT
} ShootFlags;

#define ENUM_SIZE(type) ((int)(type##_COUNT))

typedef struct ActivePowerUp {
    ShootFlags flag;
    int duration;
} ActivePowerUp;

#include "entity.h"

ShootFlags GetRandomFlag();

void OnSpeedBoostApply();
void OnSpeedBoostRemove();

void OnTripleApply();
void OnTripleRemove();

void OnHomingApply();
void OnHomingRemove();

void OnCoolingApply();
void OnCoolingRemove();


#endif
