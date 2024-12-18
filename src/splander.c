#include <stdbool.h>
#include <stdio.h>
#if defined(_WIN32) || defined(_WIN64)
#include <C:\raylib\raylib\src\raylib.h>
#else
#include "raylib.h"
#endif
#include "gui.h"

typedef struct {
	Vector2 pos;
	Vector2 size;
} Player;

typedef struct {
	Player player;
	Gui gui;
	bool is_paused;
	bool should_close;
} GameState;

// Update and draw one frame
static void UpdateDrawFrame(GameState* gs);

int main() {
	SetRandomSeed(0x69420);
	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Splander");
	SetExitKey(KEY_F1);
	SetTargetFPS(60);

	GameState gs = { 0 };
	GUIInit(&gs.gui);
	gs.player.size.x = 50;
	gs.player.size.y = 50;
	gs.player.pos.x = GetScreenWidth() / 2.0f - (gs.player.size.x / 2);
	gs.player.pos.y = GetScreenHeight() / 2.0f - (gs.player.size.y / 2);
	gs.is_paused = true;

	while (!WindowShouldClose() && !gs.should_close) {
		UpdateDrawFrame(&gs);
	}

	CloseWindow();

	return 0;
}

void GUIFunctionEval(GameState* gs) {
	if (gs->gui.sel_page == 0) {
		switch (gs->gui.sel_button) {
		case 0:
			gs->is_paused = !gs->is_paused;
			break;
		case 1:
		case 2:
			printf("[Splander LOG] Button no Function!\n");
			break;
		case 3:
			gs->should_close = true;
			break;
		default:
			printf("[Splander ERROR] button has no function assigned! \n");
			break;
		}
	}
}

// Update and draw game frame
static void UpdateDrawFrame(GameState* gs) {

	if (!gs->is_paused) {
		if (IsKeyDown(KEY_RIGHT)) gs->player.pos.x += 2.0f;
		if (IsKeyDown(KEY_LEFT))  gs->player.pos.x -= 2.0f;
		if (IsKeyDown(KEY_UP))    gs->player.pos.y -= 2.0f;
		if (IsKeyDown(KEY_DOWN))  gs->player.pos.y += 2.0f;
	}
	else {
		if (IsKeyPressed(KEY_UP)) {
			gs->gui.sel_button -= 1;
			if (gs->gui.sel_button < 0) {
				gs->gui.sel_button = gs->gui.items[gs->gui.sel_page].count - 1;
			}
		}
		if (IsKeyPressed(KEY_DOWN)) {
			gs->gui.sel_button += 1;
			if (gs->gui.sel_button >= gs->gui.items[gs->gui.sel_page].count) {
				gs->gui.sel_button = 0;
			}
		}
		if (IsKeyPressed(KEY_ENTER)) {
			GUIFunctionEval(gs);
		}
	}

	if (IsKeyPressed(KEY_ESCAPE)) gs->is_paused = !gs->is_paused;

	BeginDrawing();
	ClearBackground(GetColor(0x1E1E1EFF));

	DrawRectangleV(gs->player.pos, gs->player.size, GetColor(0xffffff));

	DrawFPS(10, 10);

	if (gs->is_paused) {
		GUIDraw(gs->gui);
	}

	EndDrawing();
}

