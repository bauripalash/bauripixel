#ifndef GUI_H
#define GUI_H

#include "tabs.h"
#include "widgets/menubar.h"
#include "widgets/statusbar.h"

typedef struct GuiGlobalState {
    MenuBarState menubar;
    StatusBarState statusbar;

} GuiGlobalState;

GuiGlobalState *NewGuiState();
void FreeGuiState(GuiGlobalState *state);

typedef struct Gui {
    TabList *tabList;
    TabObj *curTab;
    char *curFilePath;
    GuiGlobalState *state;
} Gui;

Gui *NewGui();
void FreeGui(Gui *gui);

#endif // GUI_H
