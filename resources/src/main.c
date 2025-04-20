#include "raylib.h"
#include "entity.h"
#include "player.h"
#include "game.h"
#include "constants.h"
#include "gameScreen.h"
#include "dataLoader.h"
#include "decoration.h"
#include "callBackManager.h"
#include "debug.h"
#include "UI.h"
#include "powerUp.h"

#include <stdio.h>

// SCREEN PARAMETERS
static int baseScreenWidth = SCREEN_WIDTH;
static int baseScreenHeight = SCREEN_HEIGHT;
static const char * screenTitle = "Star Shooter v1.0";

//Function Declaration
	//Core Game
	static void UpdateGame(void);
	static void DrawMenu();
	static void DrawGameplay(Camera2D* camera);
	static void DrawGameover();

ScreenType currentScreen = SCREEN_MENU;

int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

	InitWindow(baseScreenWidth, baseScreenHeight, screenTitle);
	InitAudioDevice();
	SetTargetFPS(60);

	Image icon = LoadImage("config/meta/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

	SetExitKey(0);

	AddZone(baseScreenWidth, baseScreenHeight, GAMEPLAY_ZONE, 1);
	AddZone(baseScreenWidth * 1.75, baseScreenHeight * 3.5, DELETION_ZONE, 1);
	AddZone(baseScreenWidth, baseScreenHeight, SPAWNER_ZONE, 1);

	Camera2D* camera = InitCameraScaler();

	gameData.debugFlags = 0;

	LoadDataFromJson("config/entities.json");
	LoadDataFromJson("config/sounds.json");
	LoadPowerUpIcons();
	LoadButtonData();

	RegisterAllCallbacks();

	InitPlayer();

	while(!WindowShouldClose()) {
		BeginDrawing();

		if (IsKeyPressed(KEY_ESCAPE)) {
			CloseWindow();
			break;
		}

		if (IsKeyPressed(KEY_F))
			ToggleBorderlessWindowed();

		ClearBackground((Color){ 10, 0, 20, 255 });

        AdjustCamera();

        // DRAWING GAME

		switch (currentScreen)
		{
		case SCREEN_GAME:
			DrawGameplay(camera);
			UpdateGame();
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
	ZoneUpdate();
	SpawnerUpdate();
	DecorationSpawner();
	DebugUpdate();

	//Обновление сущностей
	EntityUpdate();
}

void DrawMenu() {
	ButtonRender(BUTTON_START, (Vector2){GetScreenWidth()/2, GetScreenHeight()/2+75});
	ButtonRender(BUTTON_EXIT, (Vector2){GetScreenWidth()/2, GetScreenHeight()/2+150});
	DrawText("Move: -ARROWS-\nShoot: -Z-\nexit the game: -ESC-", 10, 10, 30, WHITE);
}

void DrawGameplay(Camera2D* camera)
{
	BeginMode2D(*camera);

	//Отрисовка элементов игры
	DrawZones();
	DrawEntities();

	EndMode2D();

	DrawUI();
}

void DrawGameover() {
	ButtonRender(BUTTON_MENU, (Vector2){GetScreenWidth()/2, GetScreenHeight()/2+75});
	ButtonRender(BUTTON_RETRY, (Vector2){GetScreenWidth()/2, GetScreenHeight()/2+150});
}