#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"

typedef enum EntityType {
    PLAYER = 0,
    ENEMY = 1,
    BULLET = 2,
    POWER_UP = 3
} EntityType;

typedef enum EnemyVariant {
    ENEMY_ASTEROID = 0,
    ENEMY_EXPLOSIVE_ASTEROID = 1,
    ENEMY_COUNT,
} EnemyVariant;

typedef enum BulletVariant {
    BULLET_NORMAL = 0,
    BULLET_ENEMY = 1,
    BULLET_COUNT,
} BulletVariant;

typedef enum PowerUpVariant {
    BOOST_SPEED,
    BOOST_TRIPLE_SHOT,
    BOOST_COOLING,
    BOOST_COUNT,
} PowerUpVariant;

typedef struct Entity {
    Rectangle rec;
    int speed;
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    float collisionRadius;

    Color color;

    EntityType type;
    int variant;

    Texture2D sprite;
    char spritePath[128];
    float spriteRotation;
    float spriteRotationSpeed;
    bool shouldRotate;

    int health;

    union
    {
        struct enemy
        {
            int score;
        } enemy;

        struct bullet
        {
            int damage;
            int homingRadius;
        } bullet;

        struct toPlayer
        {
            bool isShooting;

            float fireDelay;
            float overheat;

            int score;
            int shootFlags;
            int iFrames;
        } toPlayer;

        struct {
            float duration;
        } powerUp;

        struct particle
        {
            int timeout;
        } toParticle;
    };
} Entity;


void EntityUpdate();

void TakeDamage(Entity* entity, int damage);

void BulletUpdate(Entity* bullet);

void EnemyUpdate(Entity* enemy);

//Функции для коллбэков
void OnEnemyKill(void* data);
void OnExplosiveAsteroidKill(void* data);

void DrawEntities();

#endif // ENTITY_H