#include "entity.h"
#include "game.h"
#include "player.h"
#include "gameScreen.h"
#include "constants.h"
#include "raymath.h"
#include "callBackManager.h"
#include "sfxManager.h"

#include <stdio.h>
#include <stdlib.h>

void EntityUpdate()
{
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* entity = GetGameEntities()[i];

        if (entity != NULL) {

            switch (entity->data.type)
            {
                case ENTITY_PLAYER:
                    UpdatePlayer();
                    break;
                case ENTITY_BULLET:
                    BulletUpdate(entity);
                    break;
                case ENTITY_ENEMY:
                    EnemyUpdate(entity);
                    break;
                case ENTITY_PARTICLE:
                    ParticleUpdate(entity);
                    break;
                default:
                    break;
            }

            entity->frameCount++;

            if (!IsInsideZone(entity->rec, DELETION_ZONE)) {
                //printf("INFO: Entity Deleted!\n");
                if (entity->data.type != ENTITY_PLAYER) KillEntity(entity);
            }

            entity->rec.width = SPRITE_SCALE * (entity->size.x*12);
            entity->rec.height = SPRITE_SCALE * (entity->size.y*12);

            entity->rec.x = entity->position.x - (entity->rec.width/2);
            entity->rec.y = entity->position.y - (entity->rec.width/2);

            entity->position.x += (entity->velocity.x * entity->data.speed) * GetFrameTime() * GAME_TICKRATE;
            entity->position.y += (entity->velocity.y * entity->data.speed) * GetFrameTime() * GAME_TICKRATE;
        }
    }
}

void TakeDamage(Entity* entity, int damage) {

    switch (entity->data.type)
    {
    case ENTITY_PLAYER:
        if (entity->data.toPlayer.iFrames <= 0)
        {
            ShakeScreen(0.15, 15);
            entity->data.maxHealth -= damage;
            entity->data.toPlayer.iFrames = 60;
            SFXPlay(PLAYER_DAMAGE, 1, 1, 0);
        }
        break;
    default:
        entity->data.maxHealth -= damage;
        break;
    }

    if (entity->data.maxHealth <= 0) {
        if (entity->data.type == ENTITY_ENEMY) {
            CallCallbacks(POST_ENEMY_DEATH, (void*)entity);  // Передаем entity как void*
            KillEntity(entity);
            OnEnemyKill((void*)entity);
        } else if (entity->data.type == ENTITY_PLAYER) {
            ChangeScreen(SCREEN_GAMEOVER);
        }
    }
}

void BulletUpdate(Entity* bullet)
{

    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* entity = GetGameEntities()[i];
        if (entity != NULL) {
            if (entity->data.type != ENTITY_BULLET && entity->data.type != ENTITY_PARTICLE)
            {
                if (CheckCollisionCircles(entity->position, entity->data.collisionRadius, bullet->position, bullet->data.collisionRadius) &&
                    !IsInsideZone(entity->rec, SPAWNER_ZONE))
                {
                    if ((bullet->data.variant == 0 && entity->data.type == ENTITY_ENEMY) ||
                        bullet->data.variant == 1)
                    {
                        for (int i = 0; i < (2 + rand() % 3); i++)
                        {
                            float angleRad = DegreeToRadian((float)(rand() % 360));

                            Entity* particle = SpawnEntity(ENTITY_PARTICLE, 2, bullet->position, (Vector2){cosf(angleRad), sinf(angleRad)});
                        }

                        TakeDamage(entity, bullet->data.damage);
                        KillEntity(bullet);
                    }
                }
            }
        }
    }

    if (bullet->data.toBullet.homingRadius > 0)
    {
        Entity* closestEnemy = GetClosestEntity(bullet->position, bullet->data.toBullet.homingRadius, ENTITY_ENEMY);

        if (closestEnemy && !IsInsideZone(closestEnemy->rec, SPAWNER_ZONE)) {
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

void ParticleUpdate(Entity* particle)
{
    if (particle->data.toParticle.timeOut > 0)
    {
        particle->data.toParticle.timeOut--;
        if (particle->data.toParticle.timeOut < 50)
        {

            float alpha = particle->data.toParticle.timeOut/ 50.0f;
            particle->color.a = (unsigned char)(255 * alpha);
        }
    }
    else
    {
        KillEntity(particle);
    }
}

bool IsAsteroid(Entity* enemy)
{
    return (enemy->data.type == ENTITY_ENEMY && (enemy->data.variant == 0 || enemy->data.variant == 1 || enemy->data.variant == 1));
}

void OnEnemyKill(void* data)
{
    Entity* enemy = (Entity*)data;

    Entity* player = GetPlayer();
    player->data.toPlayer.score +=enemy->data.toEnemy.score;
}

void OnAsteroidKill(void* data)
{
    Entity* enemy = (Entity*)data;
    for (int i = 0; i < (2 + rand() % 3); i++)
    {
        float angleRad = DegreeToRadian((float)(rand() % 360));

        Entity* particle = SpawnEntity(ENTITY_PARTICLE, 0, enemy->position, (Vector2){cosf(angleRad), sinf(angleRad)});
    }

    SFXPlay(ENEMY_DEATH, 1, 1, 0);
    ShakeScreen(0.05, 10);
}

void OnExplosiveAsteroidKill(void* data)
{
    Entity* enemy = (Entity*)data;
    float angleStep = 360.0f / EXPLOSIVE_ASTEROID_BULLETS;

    for (int i = 0; i < EXPLOSIVE_ASTEROID_BULLETS; i++) {

        float angle = angleStep * i;

        float angleRad = DegreeToRadian(angle);

        Entity* bullet = SpawnEntity(ENTITY_BULLET, 1, enemy->position, (Vector2){cosf(angleRad), sinf(angleRad)});
    }

    for (int i = 0; i < (2 + rand() % 3); i++)
    {
        float angleRad = DegreeToRadian((float)(rand() % 360));

        Entity* particle = SpawnEntity(ENTITY_PARTICLE, 1, enemy->position, (Vector2){cosf(angleRad), sinf(angleRad)});
    }

    SFXPlay(ENEMY_DEATH_BIG, 1, 1, 0);
    ShakeScreen(0.5, 10);
}

void DrawEntities() {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* entity = GetGameEntities()[i];
        if (entity != NULL) {
            if (entity->data.type != ENTITY_PLAYER)
            {
                if (entity->data.sprite[entity->randomSpriteIndex].id > 0)
                {
                    Rectangle source = { 0, 0, entity->data.sprite[entity->randomSpriteIndex].width, entity->data.sprite[entity->randomSpriteIndex].height };
                    Rectangle dest = { entity->position.x, entity->position.y,
                                       entity->data.sprite[entity->randomSpriteIndex].width * 1,
                                       entity->data.sprite[entity->randomSpriteIndex].height * 1 };

                    Vector2 origin = { dest.width / 2, dest.height / 2 };

                    DrawTexturePro(entity->data.sprite[entity->randomSpriteIndex], source, dest, origin, entity->spriteRotation, entity->color);
                }
                else
                {
                    DrawRectangleRec(entity->rec, entity->color);
                }
            }
            else
            {
                if (entity->data.toPlayer.iFrames > 0) {

                    float blinkSpeed = 5 + (50 * (1.0f - (entity->data.toPlayer.iFrames / 60.0f)));
                    if (fmod(entity->data.toPlayer.iFrames, blinkSpeed) < blinkSpeed / 2) {
                        DrawRectangleRec(entity->rec, entity->color);
                    }
                    }
                else {
                    DrawRectangleRec(entity->rec, entity->color);
                }
            }
            DrawCircleLines(entity->position.x + entity->size.x/2,
                entity->position.y+entity->size.y/2,
                entity->data.collisionRadius, YELLOW);
        }
    }
}