#include "gameScreen.h"
#include "raylib.h"
#include "game.h"
#include "constants.h"
#include "dataLoader.h"
#include "callBackManager.h"
#include "UI.h"

#include <stdio.h>
#include <math.h>
#include <raymath.h>

void DrawUI() {
    Entity* player = GetPlayer();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Отображение счёта
    char scoreText[50];
    sprintf(scoreText, "%d", player->data.toPlayer.score);

    int fontSize = 55 * GetScaleFactor();
    Vector2 textSize = { (float)MeasureText(scoreText, fontSize), 0 };
    Vector2 textPosition = { (screenWidth - textSize.x) / 2, 10 };

    DrawText(scoreText, textPosition.x, textPosition.y, fontSize, WHITE);

    // === Шкала перегрева ===
    float overheat = player->data.toPlayer.overheat;
    float maxOverheat = 100.0f;  // Предположим максимум
    float overheatPercent = Clamp(overheat / maxOverheat, 0.0f, 1.0f);

    float barWidth = 200 * GetScaleFactor();
    float barHeight = 25 * GetScaleFactor();
    float barX = 25;
    float barY = screenHeight - 150  * GetScaleFactor();

    Color overheatColor = (HasShootFlag(SHOOT_OVERHEAT)) ? RED : ORANGE;

    DrawRectangleLines(barX, barY, barWidth, barHeight, (HasShootFlag(SHOOT_COOLING)) ? BLUE : LIGHTGRAY ); // рамка
    DrawRectangle(barX, barY, barWidth * overheatPercent, barHeight, overheatColor); // заполнение

    // === Шкала здоровья ===
    float health = player->data.health;
    float maxHealth = player->data.maxHealth;
    float healthPercent = Clamp(health / maxHealth, 0.0f, 1.0f);

    float healthBarWidth = 250 * GetScaleFactor();
    float healthBarHeight = 25 * GetScaleFactor();
    float healthBarX = 25;
    float healthBarY = screenHeight - 100 * GetScaleFactor();

    DrawRectangleLines(healthBarX, healthBarY, healthBarWidth, healthBarHeight, LIGHTGRAY); // рамка
    DrawRectangle(healthBarX, healthBarY, healthBarWidth * healthPercent, healthBarHeight, GREEN); // заполнение

    DrawActivePowerUps(player);
}

void DrawActivePowerUps() {
    Entity* player = GetPlayer();

    const int iconsPerRow = 4;
    const int spacing = 15;
    const int iconSize = 60 * GetScaleFactor();
    const int startX = 20;
    const int startY = 20;

    for (int i = 0; i < 10; i++) {
        if (player->data.toPlayer.powerUps[i].flag != 0) {
            ActivePowerUp* powerUp = &player->data.toPlayer.powerUps[i];

            Texture2D icon = GetIconForPowerUp(powerUp->flag);

            Vector2 iconPos = {
                startX + (i % iconsPerRow) * (iconSize + spacing),
                startY + (i / iconsPerRow) * (iconSize + spacing)
            };

            DrawTextureEx(icon, iconPos, 0, (float)iconSize / icon.width, WHITE);

            float durationRatio = powerUp->duration / powerUp->maxDuration;
            float barHeight = 6;
            float barY = iconPos.y + iconSize + 4;

            DrawRectangle(iconPos.x, barY, iconSize, barHeight, Fade(WHITE, 0.2f));
            DrawRectangle(iconPos.x, barY, iconSize * durationRatio, barHeight, PURPLE);
        }
    }
}

void ButtonRender(ButtonType type, Vector2 position)
{
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (i == type)
        {
            Button* b = &gameData.buttons[i];

            float scale = SPRITE_SCALE * b->size;
            float width = b->sprite.width * scale;
            float height = b->sprite.height * scale;

            // Центр → верхний левый угол
            b->position = (Vector2){ position.x - width / 2, position.y - height / 2 };

            Rectangle source = { 0, 0, b->sprite.width, b->sprite.height };
            b->rec = (Rectangle){
                b->position.x,
                b->position.y,
                width,
                height
            };

            Vector2 origin = { 0, 0 }; // Отрисовка от левого верхнего угла

            DrawTexturePro(b->sprite, source, b->rec, origin, 0, b->color);

            int textSize = b->size * 25;
            DrawText(
                b->text,
                b->rec.x + (b->rec.width - MeasureText(b->text, textSize)) / 2,
                b->rec.y + (b->rec.height - textSize) / 2,
                textSize,
                WHITE
            );



            bool isCurrentlySelected = (CheckCollisionPointRec(GetMousePosition(), b->rec));

            if (isCurrentlySelected && !b->isSelected )
            {
                SFXPlay(BUTTON_SELECT, 1, 1, 0);
            }

            b->isSelected = isCurrentlySelected;

            if (isCurrentlySelected) {
                DrawRectangleLines(b->rec.x, b->rec.y, b->rec.width, b->rec.height, WHITE);
                b->color = b->selectionColor;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    SFXPlay(BUTTON_PRESS, 1, 1, 0);
                    CallCallbacks(POST_BUTTON_CLICK, (void* )b->type);
                }
            }
            else
            {
                b->color = b->defaultColor;
            }
        }
    }
}

void OnStartClick(void* data)
{
    ChangeScreen(SCREEN_GAME);
    RestartGame();
}

void OnExitClick(void* data)
{
    CloseWindow();
}

void OnRetryClick(void* data)
{
    ChangeScreen(SCREEN_GAME);
    RestartGame();
}

void OnMenuClick(void* data)
{
    ChangeScreen(SCREEN_MENU);
}