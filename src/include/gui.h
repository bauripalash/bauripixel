#ifndef GUI_H
#define GUI_H

#include "tabs.h"
#include "widgets/menubar.h"
#include "widgets/statusbar.h"
#include "windows/about.h"
#include "windows/newsprite.h"
#include <stdbool.h>

typedef struct GuiGlobalState {
    MenuBarState menubar;
    StatusBarState statusbar;
    WNewSpriteState newsprite;
    WAboutState about;

} GuiGlobalState;

GuiGlobalState *NewGuiState();
void FreeGuiState(GuiGlobalState *state);

typedef struct Gui {
    TabList *tabList;
    TabObj *curTab;
    char *curFilePath;
    GuiGlobalState *state;
    bool popup;
} Gui;

Gui *NewGui();
void FreeGui(Gui *gui);

#endif // GUI_H
