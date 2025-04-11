#include "player.h"
#include "powerUp.h"
#include "entity.h"
#include "gameScreen.h"
#include "constants.h"
#include "game.h"
#include "sfxManager.h"
#include "callBackManager.h"
#include "raylib.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void InitPlayer() {
    Entity* player = GetPlayer();

    player->data.type = ENTITY_PLAYER;

    player->data.speed = GAME_TICKRATE;
    player->defaultColor = WHITE;
    player->size = (Vector2){1,1};

    player->position = (Vector2){GetZoneByType(GAMEPLAY_ZONE)->baseWidth/2, GetZoneByType(GAMEPLAY_ZONE)->baseHeight/2};

    player->data.toPlayer.maxFireDelay = NORMAL_FIREDELAY;
    player->data.toPlayer.fireDelay = 0;
    player->data.toPlayer.overheat = 0;
    player->data.toPlayer.score = 0;
    player->data.toPlayer.shootFlags = 0;
    player->data.maxHealth = 100;
    player->data.health = player->data.maxHealth;
    player->data.toPlayer.iFrames = 0;
    player->data.collisionRadius = 30;

    player->zIndex = 10;

    memset(player->data.toPlayer.powerUps, 0, sizeof(player->data.toPlayer.powerUps));

    GetGameEntities()[0] = player;
}

void DeHeat(float amount) {
    Entity* player = GetPlayer();
    if ((player->data.toPlayer.overheat - amount) < 0){
        player->data.toPlayer.overheat = 0;
    }
    else
    {
        player->data.toPlayer.overheat-=amount;
    }
}

void UpdatePlayer() {
    Entity* player = GetPlayer();

    Vector2 newPos = player->position;
    Zone* zone = GetZoneByType(GAMEPLAY_ZONE);

    //Передвижение
    if (IsKeyDown(KEY_RIGHT)) newPos.x += player->data.speed * GetFrameTime();
    if (IsKeyDown(KEY_LEFT))  newPos.x -= player->data.speed * GetFrameTime();
    if (IsKeyDown(KEY_UP))    newPos.y -= player->data.speed * GetFrameTime();
    if (IsKeyDown(KEY_DOWN))  newPos.y += player->data.speed * GetFrameTime();

    //Ограничение движение зоной
    if (newPos.x < zone->rec.x) newPos.x = zone->rec.x;
    if (newPos.x + player->size.x > zone->rec.x + zone->rec.width)
        newPos.x = zone->rec.x + zone->rec.width - player->size.x;

    if (newPos.y < zone->rec.y) newPos.y = zone->rec.y;
    if (newPos.y + player->size.y > zone->rec.y + zone->rec.height)
        newPos.y = zone->rec.y + zone->rec.height - player->size.y;

    player->position = newPos;
    player->rec.x = player->position.x - (player->rec.width/2);
    player->rec.y = player->position.y - (player->rec.height/2);

    //Стрельба
    if (IsKeyDown(KEY_Z))
    {
        Shoot();
    }
    else player->data.toPlayer.isShooting = false;

    if (!player->data.toPlayer.isShooting || HasShootFlag(SHOOT_OVERHEAT))
    {
        if (player->data.toPlayer.overheat > 0)
        {
            DeHeat(0.2f);
            if (player->data.toPlayer.overheat <= 0)
            {
                ClearShootFlag(SHOOT_OVERHEAT);
            }
        }
    }

    UpdatePowerUps();

    if (HasShootFlag(SHOOT_COOLING))
    {
        DeHeat(2.0f);
        if (HasShootFlag(SHOOT_OVERHEAT))
        {
            ClearShootFlag(SHOOT_OVERHEAT);
        }
    }

    if (player->frameCount % 3 == 0)
    {
        float angle = DegreeToRadian(70 + (rand() % 41));

        Vector2 pos;
        pos.x = player->position.x;
        pos.y = player->position.y + 20;

        Entity* particle = SpawnEntity(ENTITY_PARTICLE, 2, pos, (Vector2){cosf(angle), sinf(angle)});
        particle->defaultColor = ORANGE;
        particle->zIndex = 15;
    }
    if (player->data.toPlayer.iFrames > 0) player->data.toPlayer.iFrames--;
    if (player->data.toPlayer.fireDelay > 0) player->data.toPlayer.fireDelay--;
}

void Shoot()
{
    Entity* player = GetPlayer();

    if (player->data.toPlayer.fireDelay == 0)
    {
        SpawnEntity(ENTITY_PARTICLE, 2, player->position, (Vector2){0, -1});
        SFXPlay(SHOOT_SOUND, 0.25f, 1+ player->data.toPlayer.overheat/MAX_OVERHEAT, 0.0f);
        player->data.toPlayer.isShooting = true;

        Entity* bullets[3] = {NULL, NULL, NULL};
        bullets[0] = SpawnEntity(ENTITY_BULLET, 0, player->position, (Vector2){0,-1});

        if (HasShootFlag(SHOOT_TRIPLE_SHOT))
        {
            bullets[1] = SpawnEntity(ENTITY_BULLET, 0, player->position, (Vector2){0.5, -0.75});
            bullets[2] = SpawnEntity(ENTITY_BULLET, 0, player->position, (Vector2){-0.5, -0.75});

        }

        if (HasShootFlag(SHOOT_OVERHEAT)) player->data.toPlayer.fireDelay = player->data.toPlayer.maxFireDelay*3;
        else player->data.toPlayer.fireDelay = player->data.toPlayer.maxFireDelay;

        if (!HasShootFlag(SHOOT_COOLING))
        {
            if (!HasShootFlag(SHOOT_OVERHEAT) && player->data.toPlayer.overheat<MAX_OVERHEAT)
            {
                player->data.toPlayer.overheat+=1;

                if (player->data.toPlayer.overheat>=MAX_OVERHEAT)
                {
                    AddShootFlag(SHOOT_OVERHEAT);
                }
            }
        }

        for (int i = 0; i < 3; i++)
        {
            if (bullets[i] == NULL) continue;
            Entity* b = bullets[i];

            if (HasShootFlag(SHOOT_HOMING))
            {
                b->data.toBullet.homingRadius = 350;
            }
        }
    }
}

void AddPowerUps(ShootFlags flag, int duration) {
    Entity* player = GetPlayer();

    SFXPlay(POWER_UP_PICKUP, 0.55, 1, 0);

    for (int i = 0; i < MAX_ACTIVE_POWERUPS; i++) {
        ActivePowerUp* pu = &player->data.toPlayer.powerUps[i];

        if (pu->flag == flag && pu->duration > 0) {
            pu->duration = duration;
            pu->maxDuration = duration;
            return;
        }
    }

    for (int i = 0; i < MAX_ACTIVE_POWERUPS; i++) {
        ActivePowerUp* pu = &player->data.toPlayer.powerUps[i];

        if (pu->duration <= 0) {
            pu->flag = flag;
            pu->duration = duration;
            pu->maxDuration = duration;

            CallCallbacks(POST_POWER_UP_PICKUP, (void*)(intptr_t)flag);
            player->data.toPlayer.shootFlags |= flag;
            return;
        }
    }
}

void UpdatePowerUps() {
    Entity* player = GetPlayer();
    for (int i = 0; i < MAX_ACTIVE_POWERUPS; i++) {
        ActivePowerUp* pu = &player->data.toPlayer.powerUps[i];

        if (pu->duration > 0) {
            pu->duration--;

            if (pu->duration <= 0) {
                // Снять флаг
                player->data.toPlayer.shootFlags &= ~pu->flag;

                CallCallbacks(POST_POWER_UP_REMOVE, (void*)(intptr_t)pu->flag);
                SFXPlay(POWER_UP_END, 0.55, 1, 0);

                pu->flag = 0;
            }
        }
    }
}

// SHOOT FLAGS:

//Начисляет флаг стрельбы игроку
void AddShootFlag(ShootFlags flag)
{
    Entity* player = GetPlayer();
    player->data.toPlayer.shootFlags |= flag;
}

//Есть ли у игрока флаг стрельбы
bool HasShootFlag(ShootFlags flag)
{
    Entity* player = GetPlayer();
    return (player->data.toPlayer.shootFlags & flag) != 0;
}

//Очищает флаг стрельбы игрока
void ClearShootFlag(ShootFlags flag)
{
    Entity* player = GetPlayer();
    player->data.toPlayer.shootFlags &= ~flag;
}