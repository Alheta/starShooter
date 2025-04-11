#ifndef CALLBACK_MANAGER_H
#define CALLBACK_MANAGER_H

typedef enum {
    POST_ENEMY_DEATH,
    POST_PLAYER_DAMAGE,
    POST_POWER_UP_PICKUP,
    POST_POWER_UP_REMOVE,
    POST_BUTTON_CLICK,
} CallbackType;

typedef struct {
    CallbackType type;
    void (*callback)(void*);
    void* filter_data;
} CallbackEntry;


#define MAX_CALLBACKS 25

void AddCallback(CallbackType type, void (*callback)(void*), void* filter_data);

void CallCallbacks(CallbackType type, void* data);

void RegisterAllCallbacks();

#endif // CALLBACK_MANAGER_H
