#ifndef ENTITY_H
#define ENTITY_H

#include "powerUp.h"
#include "raylib.h"

typedef enum EntityType {
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_BULLET,
    ENTITY_POWERUP,
    ENTITY_PARTICLE,
    ENTITY_DECORATION,
} EntityType;

//Данные сущностей
typedef struct EntityData {
    EntityType type; //Тип сущености | Игрок, враг, пуля, усиление, частица
    int variant; //Вариант сущности

    float maxHealth; // Максимальное здоровье сущности
    float speed; // Скорость сущности
    float damage; // Урон сущности
    float collisionRadius; // Радиус коллизии сущности

    Texture2D sprite[10]; // Список из возможных спрайтов
    int spriteCount; // Кол-во занятых слотов списка спрайтов

    union {
        // Для игрока
        struct {
            int score;
            int fireDelay;
            int maxFireDelay;
            float overheat;
            int shootFlags;
            int iFrames;
            bool isShooting;

            ActivePowerUp powerUps[10];
        } toPlayer;

        // Для врагов
        struct {
            int scoreThreshold;
            float weight;
            int score;
        } toEnemy;

        // Для пуль
        struct {
            int homingRadius;
        } toBullet;

        // Для усилений
        struct {
            float timeOut;
            float duration;
            int assignedFlag;
        } toPowerUp;

        // Для частиц
        struct {
            float timeOut;
        } toParticle;

        // Для декора
        struct {
            float weight;
        } toDecoration;

    };
} EntityData;

typedef struct Entity {
    EntityData data;

    Vector2 position;
    Vector2 velocity;

    Color defaultColor;
    Color color;
    Rectangle rec;
    Vector2 size;

    int zIndex;

    float spriteRotation;
    float spriteRotationSpeed;
    int randomSpriteIndex;
    bool shouldRotate;

    int flashTimer;
    int shakeTimer;
    Vector2 shakePos;
    int frameCount;
} Entity;

Entity* GetPlayer();


void EntityUpdate();
void TakeDamage(Entity* entity, int damage);
void BulletUpdate(Entity* bullet);
void PowerUpUpdate(Entity* bullet);
void EnemyUpdate(Entity* enemy);
void ParticleUpdate(Entity* particle);

bool Collides(Entity* entity1, Entity* entity2);
bool IsVulnerableEntity(Entity* entity);
bool IsAsteroid(Entity* entity);

//Функции для коллбэков
void OnEnemyKill(void* data);
void OnAsteroidKill(void* data);
void OnExplosiveAsteroidKill(void* data);
void OnPoweredAsteroidKill(void* data);

void DrawEntities();

#endif // ENTITY_H