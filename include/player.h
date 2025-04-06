#ifndef PLAYER_H
#define PLAYER_H

#include "powerUp.h"
#include "raylib.h"

void InitPlayer();
void UpdatePlayer();
void DrawUI();
void Shoot();

void DeHeat(float amount);

void AddPowerUps(ShootFlags flags, int duration);
void UpdatePowerUps();

void AddShootFlag(ShootFlags flag);
bool HasShootFlag(ShootFlags flag);
void ClearShootFlag(ShootFlags flag);

#endif // PLAYER_H