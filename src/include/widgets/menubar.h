#ifndef MENUBAR_H
#define MENUBAR_H

#include "../menuinfo.h"
#include "widget.h"
#include <stdbool.h>

// Menubar State Object
typedef struct MenuBarState {
	// Widget prop base
    WidgetProp prop;
	
	// Menubar height
    float height;
	// Font for easy access
    Font font;

	// Is menu open
    bool menuOpen;
	// Is export menu open
    bool exportMenuOpen;

    // TopMenuEntry menus[TMENU_COUNT];
    TopMenuInfo curMenu;

} MenuBarState;

// Create new menubar state
MenuBarState NewMenuBar();
// Draw Menubar and return menu action (New file, open file etc)
// See <menuinfo.h>
MenuAction MenuBar(MenuBarState *state, double dt);

#endif
