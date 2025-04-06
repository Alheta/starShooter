#ifndef DEBUG_H
#define DEBUG_H

#include "raylib.h"

typedef enum DebugFlags {
    DEBUG_SHOW_HITBOXES = 1 << 0,
} DebugFlags;

void AddDebugFlag(DebugFlags flag);
bool HasDebugFlag(DebugFlags flag);
void ClearDebugFlag(DebugFlags flag);

void DebugUpdate();

#endif // DEBUG_H