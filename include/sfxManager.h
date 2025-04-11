#ifndef SFXMANAGER_H
#define SFXMANAGER_H

typedef enum SoundID {
    SHOOT_SOUND,
    PLAYER_DAMAGE,
    ENEMY_DEATH,
    ENEMY_DEATH_BIG,
    ENEMY_DEATH_POWERED,
    POWER_UP_PICKUP,
    POWER_UP_END,
    BUTTON_SELECT,
    BUTTON_PRESS,
    SOUND_COUNT_ENUM
} SoundID;

typedef struct {
    Sound variants[10];
    int variantCount;
} SoundData;

void SFXPlay(SoundID soundGroup, float volume, float pitch, float pan);

#endif // SFXMANAGER_H
