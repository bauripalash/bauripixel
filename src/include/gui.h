#ifndef GUI_H
#define GUI_H

#include "tabs.h"
#include "widgets/menubar.h"
#include "widgets/statusbar.h"
#include "windows/about.h"
#include "windows/newsprite.h"
#include <stdbool.h>

// Main Gui States
// Here only window and widget states which are present which are common 
// across tabs
typedef struct GuiGlobalState {
	// Menubar state
    MenuBarState menubar;
	// Statusbar state
    StatusBarState statusbar;
	// New Sprite Window State
    WNewSpriteState newsprite;
	// About Window State
    WAboutState about;

} GuiGlobalState;

// Create New Gui State
GuiGlobalState *NewGuiState();
// Free the Gui State
void FreeGuiState(GuiGlobalState *state);

// Main App Gui
typedef struct Gui {
	// List of tabs
    TabList *tabList;
	// Current active tab
    TabObj *curTab;
	// File path of the opened sprite
    char *curFilePath;
	// Global Gui States
    GuiGlobalState *state;
	// If any popup is active
    bool popup;
} Gui;

// Create New GUI
Gui *NewGui();
// Free the GUI object
void FreeGui(Gui *gui);

#endif // GUI_H
