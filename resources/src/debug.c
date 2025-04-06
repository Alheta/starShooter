#include "debug.h"
#include "dataLoader.h"
#include <stdio.h>

void AddDebugFlag(DebugFlags flag)
{
    gameData.debugFlags |= flag;
}

bool HasDebugFlag(DebugFlags flag)
{
    return (gameData.debugFlags & flag) != 0;
}

void ClearDebugFlag(DebugFlags flag)
{
    gameData.debugFlags &= ~flag;
}

void DebugUpdate()
{
    if (IsKeyPressed(KEY_ONE))
    {
        if (!HasDebugFlag(DEBUG_SHOW_HITBOXES)) {
            AddDebugFlag(DEBUG_SHOW_HITBOXES); }
        else {
            ClearDebugFlag(DEBUG_SHOW_HITBOXES);
        }
    }
}