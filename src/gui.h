#ifndef _GUI_H_
#define _GUI_H_

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

void GUIInit(Gui* gui);
void GUIDraw(Gui gui);

#endif
