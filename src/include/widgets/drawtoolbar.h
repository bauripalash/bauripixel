#ifndef DRAWTOOLBAR_H
#define DRAWTOOLBAR_H

#include "../../external/raylib.h"
#include "../drawtools.h"
#include "widget.h"

#define DEFAULT_DT_BTN_SIZE 50
#define DEFAULT_DTBAR_WIDTH 80

typedef struct {
    WidgetProp prop;
    Vector2 anchor;

    Vector2 toolBtnMargin;
    Vector2 toolbarPadding;

    DrawTool currentTool;
} DrawToolBarState;

DrawToolBarState NewDrawToolBar();
void FreeDrawToolBar(DrawToolBarState *state);
int DrawToolbar(DrawToolBarState *state);

#endif
