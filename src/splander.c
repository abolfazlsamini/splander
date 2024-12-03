#include <stdbool.h>
#include <stdio.h>
#if defined(_WIN64)
#include <C:\raylib\raylib\src\raylib.h>

#elif defined(_WIN32)
#include <C:\raylib\raylib\src\raylib.h>
#else
#include "raylib.h"
#endif
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

typedef struct {
    Vector2 pos;
    Vector2 size;
} Player;

typedef struct {
    char lable[64];
} GuiButton;

typedef struct {
    int count;
    int capacity;
    GuiButton* items;
} GuiPage;

typedef struct {
    int sel_button;
    int sel_page;

    int count;
    int capacity;
    GuiPage* items;
} Gui;

typedef struct {
    Player player;
    Gui gui;
} GameState;

bool IsPaused = true;
bool ShouldClose = false;

// Update and draw one frame
static void UpdateDrawFrame(GameState* gs);

static void SetGUI(GameState* gs) {

    GuiPage gui_page1 = {0};

    da_append(&gui_page1, (GuiButton) {.lable = "RESUME"});
    da_append(&gui_page1, (GuiButton) {.lable = "CREATE LOBBY"});
    da_append(&gui_page1, (GuiButton) {.lable = "JOIN LOBBY"});
    da_append(&gui_page1, (GuiButton) {.lable = "EXIT GAME"});

    da_append(&gs->gui, gui_page1);
}

static void GUIFunctionEval(Gui gui) {
    if (gui.sel_page == 0) {
        switch (gui.sel_button) {
            case 0:
                IsPaused = !IsPaused;
                break;
            case 1:
            case 2:
                printf("[Splander LOG] Button no Function!\n");
                break;
            case 3:
                ShouldClose = true;
                break;
            default:
                printf("[Splander ERROR] button has no function assigned! \n");
                break;
        }
    }
}

static void DrawGUI(GameState* gs) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GetColor(0x000000C0));

    GuiPage page = gs->gui.items[gs->gui.sel_page];
    int text_line = 50;
    for (int i = 0; i < page.count; i++) {
        Color btn_color = WHITE;
        if (gs->gui.sel_button == i) {
            btn_color = BLUE;
        }
        DrawText(page.items[i].lable, 100, text_line + (i * 50), 25, btn_color);
    }
}


int main() {
    SetRandomSeed(0x69420);
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Splander");
    SetExitKey(KEY_F1);
    SetTargetFPS(60);

    GameState gs = {0};
    SetGUI(&gs);
    gs.player.size.x = 50;
    gs.player.size.y = 50;
    gs.player.pos.x = GetScreenWidth() / 2.0f - (gs.player.size.x / 2);
    gs.player.pos.y = GetScreenHeight() / 2.0f - (gs.player.size.y / 2);

    while(!WindowShouldClose() && !ShouldClose) {
        UpdateDrawFrame(&gs);
    }

    CloseWindow();

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(GameState* gs) {

    if (!IsPaused) {
        if (IsKeyDown(KEY_RIGHT)) gs->player.pos.x += 2.0f;
        if (IsKeyDown(KEY_LEFT))  gs->player.pos.x -= 2.0f;
        if (IsKeyDown(KEY_UP))    gs->player.pos.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN))  gs->player.pos.y += 2.0f;
    } else {
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
            GUIFunctionEval(gs->gui);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) IsPaused = !IsPaused;

    BeginDrawing();
    ClearBackground(GetColor(0x1E1E1EFF));

    DrawRectangleV(gs->player.pos, gs->player.size, GetColor(0xAC29E2FF));

    DrawFPS(10, 10);

    if (IsPaused) {
        DrawGUI(gs);
    }

    EndDrawing();
}

