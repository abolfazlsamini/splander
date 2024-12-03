#include <assert.h>
#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
#include <C:\raylib\raylib\src\raylib.h>
#else
#include "raylib.h"
#endif
#include "gui.h"

#define NOB_DA_INIT_CAP 256
#define da_append(da, item)                                                          \
    do {                                                                                 \
        if ((da)->count >= (da)->capacity) {                                             \
            (da)->capacity = (da)->capacity == 0 ? NOB_DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Buy more RAM lol");                       \
        }                                                                                \
                                                                                         \
        (da)->items[(da)->count++] = (item);                                             \
    } while (0)


void GUIInit(Gui* gui) {

    GuiPage gui_page1 = {0};

    da_append(&gui_page1, (GuiButton) {.lable = "RESUME"});
    da_append(&gui_page1, (GuiButton) {.lable = "CREATE LOBBY"});
    da_append(&gui_page1, (GuiButton) {.lable = "JOIN LOBBY"});
    da_append(&gui_page1, (GuiButton) {.lable = "EXIT GAME"});

    da_append(gui, gui_page1);
}


void GUIDraw(Gui gui) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GetColor(0x000000C0));

    GuiPage page = gui.items[gui.sel_page];
    int text_line = 50;
    for (int i = 0; i < page.count; i++) {
        Color btn_color = WHITE;
        if (gui.sel_button == i) {
            btn_color = BLUE;
        }
        DrawText(page.items[i].lable, 100, text_line + (i * 50), 25, btn_color);
    }
}
