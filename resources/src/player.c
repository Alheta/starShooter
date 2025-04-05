#include "player.h"
#include "entity.h"
#include "gameScreen.h"
#include "constants.h"
#include "game.h"
#include "sfxManager.h"

#include <stdio.h>
#include <math.h>

static Entity player;

Entity* GetPlayer() {
    return &player;
}

void InitPlayer() {
    Entity* player = GetPlayer();

    player->type = PLAYER;

    player->speed = GAME_TICKRATE;
    player->color = WHITE;
    player->size = (Vector2){1,1};

    player->position = (Vector2){GetScreenWidth()/2, GetScreenHeight()/2};

    player->toPlayer.fireDelay = 0;
    player->toPlayer.overheat = 0;
    player->toPlayer.score = 0;
    player->toPlayer.shootFlags = 0;
    player->health = 100;
    player->toPlayer.iFrames = 0;

    GetGameEntities()[0] = player;
}

void UpdatePlayer() {
    Entity* player = GetPlayer();

    Vector2 newPos = player->position;
    Zone* zone = GetZoneByType(GAMEPLAY_ZONE);

    //Передвижение
    if (IsKeyDown(KEY_RIGHT)) newPos.x += player->speed * GetFrameTime();
    if (IsKeyDown(KEY_LEFT))  newPos.x -= player->speed * GetFrameTime();
    if (IsKeyDown(KEY_UP))    newPos.y -= player->speed * GetFrameTime();
    if (IsKeyDown(KEY_DOWN))  newPos.y += player->speed * GetFrameTime();

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
    else player->toPlayer.isShooting = false;

    if (!player->toPlayer.isShooting || HasShootFlag(SHOOT_OVERHEAT))
    {
        if (player->toPlayer.overheat > 0)
        {
            player->toPlayer.overheat-=0.2f;
            if (player->toPlayer.overheat <= 0)
            {
                ClearShootFlag(SHOOT_OVERHEAT);
            }
        }
    }

    if (player->toPlayer.iFrames > 0) player->toPlayer.iFrames--;
    if (player->toPlayer.fireDelay > 0) player->toPlayer.fireDelay--;
}

void Shoot()
{
    Entity* player = GetPlayer();

    if (player->toPlayer.fireDelay == 0)
    {
        SFXPlay(SHOOT_SOUND, 0.25f, 1, 0.0f);
        player->toPlayer.isShooting = true;

        Entity* bullets[3] = {NULL, NULL, NULL};
        bullets[0] = SpawnEntity(BULLET, BULLET_NORMAL, player->position, (Vector2){0,-1});

        if (HasShootFlag(SHOOT_TRIPLE_SHOT))
        {
            bullets[1] = SpawnEntity(BULLET, BULLET_NORMAL, player->position, (Vector2){0.5, -0.75});
            bullets[2] = SpawnEntity(BULLET, BULLET_NORMAL, player->position, (Vector2){-0.5, -0.75});

        }

        if (HasShootFlag(SHOOT_OVERHEAT)) player->toPlayer.fireDelay = 15;
        else player->toPlayer.fireDelay = 5;

        if (!HasShootFlag(SHOOT_OVERHEAT) && player->toPlayer.overheat<MAX_OVERHEAT)
        {
            player->toPlayer.overheat+=1;

            if (player->toPlayer.overheat>=MAX_OVERHEAT)
            {
                AddShootFlag(SHOOT_OVERHEAT);
            }
        }
        for (int i = 0; i < 3; i++)
        {
            if (bullets[i] == NULL) continue;
            Entity* b = bullets[i];

            if (HasShootFlag(SHOOT_HOMING))
            {
                b->bullet.homingRadius = 350;
            }

            if (HasShootFlag(SHOOT_OVERHEAT))
            {
                b->bullet.damage = NORMAL_BULLET_DMG / 2;
                b->size = (Vector2){0.5f, 0.5f};
            }
        }
    }
}

void DrawUI() {
    Entity* player = GetPlayer();

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    char scoreText[50];
    sprintf(scoreText, "%d", player->toPlayer.score);

    Vector2 textSize = { (float)MeasureText(scoreText, 20), 0 };

    Vector2 textPosition = { (GetScreenWidth() - textSize.x) / 2, 10 };

    DrawText(scoreText, textPosition.x, textPosition.y, 55 * GetScaleFactor(), BLACK);

    //Перегрев
    Color clr;
    if (HasShootFlag(SHOOT_OVERHEAT)) clr = RED;
    else clr = BLACK;
    DrawText(TextFormat("%.2f", player->toPlayer.overheat), 25, screenHeight - 150, 55, clr);

    DrawText(TextFormat("%d", player->health), 50, screenHeight - 70, 55, clr);
}


// SHOOT FLAGS:

//Начисляет флаг стрельбы игроку
void AddShootFlag(ShootFlags flag)
{
    Entity* player = GetPlayer();
    player->toPlayer.shootFlags |= flag;
}

//Есть ли у игрока флаг стрельбы
bool HasShootFlag(ShootFlags flag)
{
    Entity* player = GetPlayer();
    return (player->toPlayer.shootFlags & flag) != 0;
}

//Очищает флаг стрельбы игрока
void ClearShootFlag(ShootFlags flag)
{
    Entity* player = GetPlayer();
    player->toPlayer.shootFlags &= ~flag;
}