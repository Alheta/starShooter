#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "player.h"
#include "entity.h"

typedef enum ButtonType {
    BUTTON_START,
    BUTTON_RETRY,
    BUTTON_EXIT,
    BUTTON_MENU,
    BUTTON_COUNT
} ButtonType;

typedef struct Button {
    Texture2D sprite;
    ButtonType type;
    Vector2 position;
    Color color;
    Color selectionColor;
    Color defaultColor;
    char* text;
    int size;
    Rectangle rec;
    bool isSelected;
} Button;

void DrawUI();
void DrawActivePowerUps();

void ButtonRender(ButtonType type, Vector2 position);

void OnStartClick(void* data);
void OnExitClick(void* data);
void OnRetryClick(void* data);
void OnMenuClick(void* data);

#endif // UI_H
