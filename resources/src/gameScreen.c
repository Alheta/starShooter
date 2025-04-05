#include "gameScreen.h"
#include "raylib.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const float zoomZoneWidth = 1280;
const float zoomZoneHeight = 720;

static Camera2D camera;

Camera2D* GetCamera() {
    return &camera;
}

Camera2D* InitCameraScaler() {
    Camera2D* camera = GetCamera();
    camera->target = (Vector2){zoomZoneWidth / 2, zoomZoneHeight / 2};
    camera->offset = (Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2};
    camera->rotation = 0.0f;
    camera->zoom = 1.0f;
    return camera;
}

float GetScaleFactor() {
    int currentScreenWidth = GetScreenWidth();
    int currentScreenHeight = GetScreenHeight();

    float scaleWidth = (float)currentScreenWidth / zoomZoneWidth;
    float scaleHeight = (float)currentScreenHeight / zoomZoneHeight;

    return ((scaleWidth < scaleHeight) ? scaleWidth : scaleHeight) - 0.25;
}

float shakeDuration = 0.0f;  // Время тряски
float shakeMagnitude = 5.0f; // Амплитуда тряски
Vector2 originalCameraPosition = {0, 0}; // Исходное положение камеры

void ShakeScreen(float duration, float magnitude) {
    Camera2D* camera = GetCamera();
    shakeDuration = duration;
    shakeMagnitude = magnitude;
    originalCameraPosition = camera->offset;
}

void AdjustCamera() {
    Camera2D* camera = GetCamera();

    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    float shakeX = 0;
    float shakeY = 0;
    if (shakeDuration > 0.0f) {
        shakeX = (rand() % 3 - 1) * shakeMagnitude;
        shakeY = (rand() % 3 - 1) * shakeMagnitude;

        camera->offset.x = shakeX;
        camera->offset.y = shakeY;

        shakeDuration -= GetFrameTime();

        if (shakeDuration <= 0.0f) {
            camera->offset = originalCameraPosition;
        }
    }

    camera->zoom = GetScaleFactor();
    camera->offset = (Vector2){windowWidth / 2 + shakeX, windowHeight / 2 + shakeY};
}

void DrawTextAnchored(const char* text, int x, int y, int fontSize, Color color, ScreenPosition anchor) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;
    Vector2 anchoredPos;

    switch (anchor)
    {
    case TOP_LEFT:
        anchoredPos = (Vector2){0,0};
        break;
    case TOP_RIGHT:
        anchoredPos = (Vector2){0,2};
        break;
    case BOTTOM_LEFT:
        anchoredPos = (Vector2){2,0};
        break;
    case BOTTOM_RIGHT:
        anchoredPos = (Vector2){2,2};
        break;
    case CENTER:
        anchoredPos = (Vector2){1,1};
        break;
    default:
        break;
    }

    // Корректируем x и y в зависимости от якоря (0 = левый/верхний, 1 = центр, 2 = правый/нижний)
    if (anchoredPos.x == 1) x -= textWidth / 2;  // Центр по X
    else if (anchoredPos.x == 2) x -= textWidth; // Правый край

    if (anchoredPos.y == 1) y -= textHeight / 2; // Центр по Y
    else if (anchoredPos.y == 2) y -= textHeight; // Нижний край

    DrawText(text, x, y, fontSize, color);
}

void ChangeScreen(ScreenType newScreen) {
    currentScreen = newScreen;
}