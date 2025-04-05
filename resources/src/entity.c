#include "entity.h"
#include "game.h"
#include "player.h"
#include "gameScreen.h"
#include "constants.h"
#include "raymath.h"
#include "callBackManager.h"

#include <stdio.h>

void EntityUpdate()
{
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* entity = GetGameEntities()[i];

        if (entity != NULL) {

            switch (entity->type)
            {
                case PLAYER:
                    UpdatePlayer();
                    break;
                case BULLET:
                    BulletUpdate(entity);
                    break;
                case ENEMY:
                    EnemyUpdate(entity);
                    break;
                default:
                    break;
            }

            if (!IsInsideZone(entity->rec, DELETION_ZONE)) {
                //printf("INFO: Entity Deleted!\n");
                if (entity->type != PLAYER) KillEntity(entity);
            }

            entity->rec.width = SPRITE_SCALE * (entity->size.x*12);
            entity->rec.height = SPRITE_SCALE * (entity->size.y*12);

            entity->rec.x = entity->position.x - (entity->rec.width/2);
            entity->rec.y = entity->position.y - (entity->rec.width/2);
        }
    }
}

void TakeDamage(Entity* entity, int damage) {

    switch (entity->type)
    {
    case PLAYER:
        if (entity->toPlayer.iFrames <= 0)
        {
            ShakeScreen(0.15, 15);
            entity->health -= damage;
            entity->toPlayer.iFrames = 60;
        }
        break;
    default:
        entity->health -= damage;
        break;
    }

    if (entity->health <= 0) {
        if (entity->type == ENEMY) {
            OnEnemyKill((void*)entity);
            CallCallbacks(POST_ENEMY_DEATH, (void*)entity);  // Передаем entity как void*
            KillEntity(entity);
        } else if (entity->type == PLAYER) {
            ChangeScreen(SCREEN_GAMEOVER);
        }
    }
}

void BulletUpdate(Entity* bullet)
{
    bullet->position.x += (bullet->velocity.x * bullet->speed) * GetFrameTime() * GAME_TICKRATE;
    bullet->position.y += (bullet->velocity.y * bullet->speed) * GetFrameTime() * GAME_TICKRATE;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* entity = GetGameEntities()[i];
        if (entity != NULL) {
            if (entity->type != BULLET)
            {
                if (CheckCollisionCircles(entity->position, entity->collisionRadius, bullet->position, bullet->collisionRadius))
                {
                    if ((bullet->variant == BULLET_NORMAL && entity->type == ENEMY) ||
                        bullet->variant == BULLET_ENEMY)
                    {
                        TakeDamage(entity, bullet->bullet.damage);
                        KillEntity(bullet);
                    }
                }
            }
        }
    }

    if (bullet->bullet.homingRadius > 0)
    {
        Entity* closestEnemy = GetClosestEntity(bullet->position, bullet->bullet.homingRadius, ENEMY);

        if (closestEnemy) {
            Vector2 direction = Vector2Subtract(closestEnemy->position, bullet->position);
            if (Vector2LengthSqr(direction) > 0.0001f) {
                direction = Vector2Normalize(direction);
                bullet->velocity = Vector2Lerp(bullet->velocity, direction, 0.25f);
            }
        }
    }
}


void EnemyUpdate(Entity* enemy)
{
    enemy->position.x += (enemy->velocity.x * enemy->speed) * GetFrameTime() * GAME_TICKRATE;
    enemy->position.y += (enemy->velocity.y * enemy->speed) * GetFrameTime() * GAME_TICKRATE;

    if (enemy->shouldRotate)
    {
        enemy->spriteRotation += enemy->spriteRotationSpeed;

        if (enemy->spriteRotation >= 360.0f) {
            enemy->spriteRotation -= 360.0f;
        } else if (enemy->spriteRotation < 0.0f) {
            enemy->spriteRotation += 360.0f;
        }
    }
}

void OnEnemyKill(void* data)
{
    Entity* enemy = (Entity*)data;

    Entity* player = GetPlayer();
    player->toPlayer.score+=enemy->enemy.score;
}

void OnExplosiveAsteroidKill(void* data)
{
    Entity* enemy = (Entity*)data;
    float angleStep = 360.0f / EXPLOSIVE_ASTEROID_BULLETS;

    for (int i = 0; i < EXPLOSIVE_ASTEROID_BULLETS; i++) {

        float angle = angleStep * i;

        float angleRad = DegreeToRadian(angle);

        Entity* bullet = SpawnEntity(BULLET, BULLET_ENEMY, enemy->position, (Vector2){cosf(angleRad), sinf(angleRad)});
        bullet->speed = 1;
    }

    ShakeScreen(0.5, 10);
}

void DrawEntities() {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* entity = GetGameEntities()[i];
        if (entity != NULL) {
            if (entity->type != PLAYER)
            {
                DrawCircleLines(entity->position.x+entity->size.x/2, entity->position.y+entity->size.y/2, 10, YELLOW);
                if (entity->sprite.id > 0)
                {
                    Rectangle source = { 0, 0, entity->sprite.width, entity->sprite.height };
                    Rectangle dest = { entity->position.x, entity->position.y,
                                       entity->sprite.width * 1,
                                       entity->sprite.height * 1 };

                    Vector2 origin = { dest.width / 2, dest.height / 2 };

                    DrawTexturePro(entity->sprite, source, dest, origin, entity->spriteRotation, WHITE);
                }
                else
                {
                    DrawRectangleRec(entity->rec, entity->color);
                }
            }
            else
            {
                if (entity->toPlayer.iFrames > 0) {

                    float blinkSpeed = 5 + (50 * (1.0f - (entity->toPlayer.iFrames / 60.0f)));
                    if (fmod(entity->toPlayer.iFrames, blinkSpeed) < blinkSpeed / 2) {
                        DrawRectangleRec(entity->rec, entity->color);
                    }
                    }
                else {
                    DrawRectangleRec(entity->rec, entity->color);
                }
            }
        }
    }
}