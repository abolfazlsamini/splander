#include <cstdint>
#if defined(_WIN32) || defined(_WIN64)
#include <C:\raylib\raylib\src\raylib.h>
#else
#include "raylib.h"
#endif
#include "gui.h"

void GUIInit(Gui* gui) {

    GuiPage gui_page1;

    gui_page1.buttons.push_back((GuiButton) {.lable = "RESUME"});
    gui_page1.buttons.push_back((GuiButton) {.lable = "CREATE LOBBY"});
    gui_page1.buttons.push_back((GuiButton) {.lable = "JOIN LOBBY"});
    gui_page1.buttons.push_back((GuiButton) {.lable = "EXIT GAME"});

    gui->items.push_back(gui_page1);
}


void GUIDraw(Gui gui) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GetColor(0x000000C0));

    GuiPage page = gui.items[gui.sel_page];
    int text_line = 50;
    for (int i = 0;(uint64_t) i < page.buttons.size(); i++) {
        Color btn_color = WHITE;
        if (gui.sel_button == i) {
            btn_color = BLUE;
        }
        DrawText(page.buttons[i].lable.c_str(), 100, text_line + (i * 50), 25, btn_color);
    }
}
