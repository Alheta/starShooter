#ifndef SFXMANAGER_H
#define SFXMANAGER_H

// Per-sound types enumeration
typedef enum {
    SHOOT_SOUND,
    SOUND_COUNT_ENUM  // Number of sound types
} SoundID;

// Structure to hold sound variants
typedef struct {
    Sound variants[10];  // Maximum 10 variants per sound
    int variantCount;    // Number of sound variants
} SoundGroupData;

void SFXPlay(SoundID soundGroup, float volume, float pitch, float pan);  // Changed name

#endif // SFXMANAGER_H
