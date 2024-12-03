#if defined(_WIN64)
#include <C:\raylib\raylib\src\raylib.h>

#elif defined(_WIN32)
#include <C:\raylib\raylib\src\raylib.h>
#else
#include "raylib.h"
#endif
#include <stdlib.h>

typedef struct {
    Vector2 pos;
    Vector2 size;
} Player;

typedef struct {
    Player player;
} GameState;

// Update and draw one frame
static void UpdateDrawFrame(GameState* gs);


int main() {
    SetRandomSeed(0x69420);
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Splander");
    SetTargetFPS(60);

    GameState* gs = malloc(sizeof(GameState));
    gs->player.size.x = 50;
    gs->player.size.y = 50;
    gs->player.pos.x = GetScreenWidth() / 2.0f - (gs->player.size.x / 2);
    gs->player.pos.y = GetScreenHeight() / 2.0f - (gs->player.size.y / 2);

    while(!WindowShouldClose()) {
        UpdateDrawFrame(gs);
    }

    free(gs);
    CloseWindow();

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(GameState* gs) {
    
    if (IsKeyDown(KEY_RIGHT)) gs->player.pos.x += 2.0f;
    if (IsKeyDown(KEY_LEFT))  gs->player.pos.x -= 2.0f;
    if (IsKeyDown(KEY_UP))    gs->player.pos.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN))  gs->player.pos.y += 2.0f;

    BeginDrawing();
        ClearBackground(GetColor(0x1E1E1EFF));

        DrawRectangleV(gs->player.pos, gs->player.size, GetColor(0xAC29E2FF));

	DrawFPS(10, 10);

	EndDrawing();
}
