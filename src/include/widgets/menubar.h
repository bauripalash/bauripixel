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

    TopMenuEntry menus[TMENU_COUNT];

} MenuBarState;

MenuBarState NewMenuBar();
MenuAction MenuBar(MenuBarState *state);

#endif
