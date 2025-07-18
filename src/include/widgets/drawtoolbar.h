#ifndef DRAWTOOLBAR_H
#define DRAWTOOLBAR_H

#include "../../external/raylib.h"
#include "../drawtools.h"
#include "widget.h"
#include <stdbool.h>

#define DEFAULT_DT_BTN_SIZE 40
#define DEFAULT_DTBAR_WIDTH 60
#define DTBAR_MARGIN_L      10
#define DTBAR_MARGIN_R      DTBAR_MARGIN_L
#define DTBAR_MARGIN_TB     10

typedef struct {
    WidgetProp prop;
    Vector2 optAnchor;
    Vector2 anchor;

    Vector2 toolBtnMargin;
    Vector2 toolbarPadding;

    DrawTool currentTool;

    int brushSize;
    bool brushSizeEdit;
} DrawToolBarState;

DrawToolBarState NewDrawToolBar();
void FreeDrawToolBar(DrawToolBarState *state);
int DrawToolbar(DrawToolBarState *state);

#endif
