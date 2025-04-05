#include "raylib.h"
#include "player.h"
#include "entity.h"
#include "game.h"
#include "constants.h"
#include "gameScreen.h"
#include "dataLoader.h"
#include "callBackManager.h"

#include <stdio.h>

// SCREEN PARAMETERS
static int baseScreenWidth = SCREEN_WIDTH;
static int baseScreenHeight = SCREEN_HEIGHT;
static const char * screenTitle = "Star Shooter v0.1";

//Function Declaration
	//Core Game
	static void UpdateGame(void);
	static void DrawMenu();
	static void DrawGameplay(Camera2D* camera);
	static void DrawGameover();
	static void RestartGame();

ScreenType currentScreen = SCREEN_MENU;

int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

	InitWindow(baseScreenWidth, baseScreenHeight, screenTitle);
	InitAudioDevice();
	SetTargetFPS(60);

	SetExitKey(0);

	AddZone(baseScreenWidth, baseScreenHeight, GAMEPLAY_ZONE, 1);
	AddZone(baseScreenWidth * 1.75, baseScreenHeight * 3, DELETION_ZONE, 1);
	AddZone(baseScreenWidth, baseScreenHeight, SPAWNER_ZONE, 1);

	Camera2D* camera = InitCameraScaler();

	LoadDataFromJson("config/entities.json", &gameData);
	LoadDataFromJson("config/sounds.json", &gameData);

	RegisterAllCallbacks();

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(DARKGRAY);
		UpdateGame();

        AdjustCamera();

        // DRAWING GAME

		switch (currentScreen)
		{
		case SCREEN_GAME:
			DrawGameplay(camera);
			break;
		case SCREEN_MENU:
			DrawMenu();
			break;
		case SCREEN_GAMEOVER:
			DrawGameover();
			break;
		}

		EndDrawing();
	}
    CloseAudioDevice();
	CloseWindow();
	return 0;
}

//Update Game
void UpdateGame()
{
	if (IsKeyPressed(KEY_B))
	{
		AddShootFlag(SHOOT_TRIPLE_SHOT);
		AddShootFlag(SHOOT_HOMING);
	}
	if (IsKeyPressed(KEY_N))
	{
		ClearShootFlag(SHOOT_TRIPLE_SHOT);
		ClearShootFlag(SHOOT_HOMING);
	}


	ZoneUpdate();
	SpawnerUpdate();

	//Обновление сущностей
	EntityUpdate();


	if (IsKeyPressed(KEY_F))
		ToggleBorderlessWindowed();

}

void DrawMenu() {
	char* mainMenuText = "Main Menu";
	char* playButtonText = "Play";
    DrawText(mainMenuText, GetScreenWidth()/2-MeasureText(mainMenuText, 30)/2, GetScreenHeight()/2-50, 30, BLACK);

	Rectangle playButton = {GetScreenWidth()/2-100, GetScreenHeight()/2+150, 200, 50 };

    DrawRectangleRec(playButton, MAROON);
    DrawText(playButtonText, playButton.x + 75, playButton.y + 15, 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), playButton)) {
        DrawRectangleLines(playButton.x, playButton.y, playButton.width, playButton.height, BLACK);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			ChangeScreen(SCREEN_GAME);
			RestartGame();
		}
    }
}

void DrawGameplay(Camera2D* camera)
{
	BeginMode2D(*camera);

	//Отрисовка элементов игры
	DrawZones();
	DrawEntities();

	DrawText("Move:ARROWS\nShoot:Z\nToggle ON/OFF tripple shot: B/N", 10, 10, 17, BLACK);
	//--

	EndMode2D();

	DrawUI();
}

void DrawGameover() {
	char* menuText = "Game Over!";
	char* menuButtonText = "Main Menu";
	char* retryButtonText = "Retry";

    DrawText(menuText, GetScreenWidth()/2-MeasureText(menuText, 30)/2, GetScreenHeight()/2-50, 30, BLACK);

	Rectangle menuButton = {GetScreenWidth()/2-100, GetScreenHeight()/2+150, 200, 50 };

    DrawRectangleRec(menuButton, MAROON);
    DrawText(menuButtonText, menuButton.x + 50, menuButton.y + 15, 20, WHITE);

	Rectangle retryButton = {GetScreenWidth()/2-100, GetScreenHeight()/2+220, 200, 50 };

	DrawRectangleRec(retryButton, MAROON);
    DrawText(retryButtonText, retryButton.x + 75, retryButton.y + 15, 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), menuButton)) {
        DrawRectangleLines(menuButton.x, menuButton.y, menuButton.width, menuButton.height, BLACK);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			ChangeScreen(SCREEN_MENU);
		}
    }
	else if (CheckCollisionPointRec(GetMousePosition(), retryButton)) {
        DrawRectangleLines(retryButton.x, retryButton.y, retryButton.width, retryButton.height, BLACK);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			ChangeScreen(SCREEN_GAME);
			RestartGame();
		}
    }
}

void RestartGame()
{
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* entity = GetGameEntities()[i];
		if (entity)
		{
			if (entity->type != PLAYER)
			{
				KillEntity(entity);
			}
		}
	}

	InitPlayer();
	InitSpawner();
}