#include "debug.h"
#include "entity.h"
#include "game.h"
#include "constants.h"
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
    if (IsKeyPressed(KEY_TWO))
    {
        for (int i = 0; i < MAX_ENTITIES; i++) {
            Entity* entity = GetGameEntities()[i];
            if (entity != NULL) {

                if (entity->data.type == ENTITY_ENEMY)
                {
                    TakeDamage(entity, entity->data.health);
                }
            }
        }
    }
}