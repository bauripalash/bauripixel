#ifndef DRAWTOOLBAR_H
#define DRAWTOOLBAR_H

#include "../../external/raylib.h"
#include "../components.h"
#include "../drawtools.h"
#include "widget.h"
#include <stdbool.h>

#define DEFAULT_DT_BTN_SIZE 40
#define DEFAULT_DTBAR_WIDTH 50
#define DTBAR_MARGIN_L      8
#define DTBAR_MARGIN_R      DTBAR_MARGIN_L
#define DTBAR_MARGIN_TB     10

typedef struct {
    WidgetProp prop;
    Vector2 anchor;
    Vector2 bottom;

    Rectangle optRect;
    Rectangle toolsRect;

    Vector2 toolBtnMargin;
    Vector2 toolbarPadding;

    DrawTool currentTool;

    int brushSize;
    int maxBrushSize;
    Font font;
    bool sliderHover;
    BrushShape brushShape;

    bool brushSizeEdit;

} DrawToolBarState;

DrawTool HandleDToolsShortcuts(DrawToolBarState *state);
DrawToolBarState NewDrawToolBar();
void SetDrawToolBarAnchor(
    DrawToolBarState *dtb, Vector2 anchor, Vector2 bottom
);
void FreeDrawToolBar(DrawToolBarState *state);
int DrawToolbar(DrawToolBarState *state);

#endif
