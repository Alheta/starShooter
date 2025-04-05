#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "raylib.h"

typedef struct Player {
    Rectangle rec;
    Vector2 position;
    Vector2 size;
    Color color;

    float speed;
    float fireDelay;
    float overheat;
    float collisionRadius;

    int shootFlags;
    int score;

    bool isShooting;
} Player;

typedef enum ShootFlags {
    SHOOT_OVERHEAT = 1 << 0,
	SHOOT_TRIPLE_SHOT =  1 << 1,
	SHOOT_HOMING = 1 << 2
} ShootFlags;

void InitPlayer();
void UpdatePlayer();
void DrawUI();
void Shoot();
Entity* GetPlayer();


void AddShootFlag(ShootFlags flag);
bool HasShootFlag(ShootFlags flag);
void ClearShootFlag(ShootFlags flag);

#endif // PLAYER_H