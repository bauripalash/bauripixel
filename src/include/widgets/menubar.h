#ifndef MENUBAR_H
#define MENUBAR_H

#include "../menuinfo.h"
#include "widget.h"
#include <stdbool.h>
typedef struct MenuBarState {
    WidgetProp prop;
    float height;
    Font font;

    bool menuOpen;
    bool exportMenuOpen;

    // TopMenuEntry menus[TMENU_COUNT];
    TopMenuInfo curMenu;

} MenuBarState;

MenuBarState NewMenuBar();
MenuAction MenuBar(MenuBarState *state, double dt);

#endif
