#ifndef MENUBAR_H
#define MENUBAR_H

#include "widget.h"
#include <stdbool.h>
typedef struct MenuBarState {
    WidgetProp prop;
    float height;
    Font font;

    bool mFileClicked;

} MenuBarState;

MenuBarState NewMenuBar();
void MenuBar(MenuBarState *state);

#endif
