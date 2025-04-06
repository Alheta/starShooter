#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"

typedef enum EntityType {
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_BULLET,
    ENTITY_POWERUP,
    ENTITY_PARTICLE,
} EntityType;

typedef struct EntityData {
    EntityType type;
    int variant;

    char* name;
    char* spritePath;
    float maxHealth;
    float speed;
    float damage;
    float collisionRadius;

    Texture2D sprite[10];
    int spriteCount;

    union {
        struct {
            int score;
            int fireDelay;
            float overheat;
            int shootFlags;
            int iFrames;
            bool isShooting;
        } toPlayer;

        // Для врагов
        struct {
            float weight;
            int score;
        } toEnemy;

        // Для пуль
        struct {
            int homingRadius;
        } toBullet;

        // Для усилений
        struct {
            float duration;
        } toPowerUp;

        // Для частиц
        struct {
            float timeOut;
        } toParticle;
    };
} EntityData;

typedef struct Entity {
    EntityData data;

    Vector2 position;
    Vector2 velocity;

    Color color;
    Rectangle rec;
    Vector2 size;

    float spriteRotation;
    float spriteRotationSpeed;
    int randomSpriteIndex;
    bool shouldRotate;

    int frameCount;
} Entity;

void EntityUpdate();
void TakeDamage(Entity* entity, int damage);
void BulletUpdate(Entity* bullet);
void EnemyUpdate(Entity* enemy);
void ParticleUpdate(Entity* particle);

bool IsAsteroid(Entity* entity);

//Функции для коллбэков
void OnEnemyKill(void* data);
void OnAsteroidKill();
void OnExplosiveAsteroidKill(void* data);

void DrawEntities();

#endif // ENTITY_H