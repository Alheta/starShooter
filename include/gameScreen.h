#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "raylib.h"

typedef enum ScreenPosition {
    TOP_LEFT,
    BOTTOM_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    CENTER
} ScreenPosition;

typedef enum ScreenType {
    SCREEN_MENU,
    SCREEN_GAME,
    SCREEN_GAMEOVER
} ScreenType;

Camera2D* GetCamera();
Camera2D* InitCameraScaler();
float GetScaleFactor();
void ShakeScreen(float duration, float magnitude);
void AdjustCamera();
void DrawTextAnchored(const char* text, int x, int y, int fontSize, Color color, ScreenPosition anchor);

extern ScreenType currentScreen;

void ChangeScreen(ScreenType newScreen);

#endif // GAMESCREEN_H