#ifndef _GUI_H_
#define _GUI_H_
#include <string>
#include <vector>

struct GuiButton {
    std::string lable;
};

struct GuiPage {
    std::vector<GuiButton> buttons;
};

struct Gui {
    int sel_button;
    int sel_page;

    std::vector<GuiPage> items;
};

void GUIInit(Gui* gui);
void GUIDraw(Gui gui);

#endif
