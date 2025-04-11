#include "UI.h"
#include "gameScreen.h"
#include "raylib.h"
#include "game.h"

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
