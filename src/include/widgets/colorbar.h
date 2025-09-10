#ifndef COLORBAR_H
#define COLORBAR_H

#include "widget.h"
#include <stdbool.h>

#define CBAR_NOCOLOR          (Color){0, 0, 0, 0}

#define CBAR_MARGIN_TOPBOTTOM 10.0f
#define CBAR_MARGIN_LEFT      10.0f
#define CBAR_MARGIN_RIGHT     CBAR_MARGIN_LEFT

#define CBAR_BOX_SIZE         30.0f
#define CBAR_INIT_WIDTH       98.0f
#define CBAR_MIN_WIDTH        70.0f
#define CBAR_INIT_HEIGHT      300.0f
#define CBAR_HANDLE_THICKNESS 20.0f

#define CB_STATUS_NONE        1
#define CB_STATUS_HDRAG       2
#define CB_STATUS_VDRAG       3

#define ColorsCount(c)        (sizeof(c) / sizeof(Color))

typedef struct {
    WidgetProp prop;
    int prevIndex;
    int currentIndex;
    Color currentColor;
    Vector2 anchor;
    Vector2 bottom;

    // Will be memory allocated
    // will be used with stb_ds
    Color *colors;
    int colorCount;
    Vector2 scroll;
    Rectangle view;

    int boxSize;
    int boxSpacing;

    Rectangle usableRect;
    Rectangle usedRect;
    int maxColumns;
    int usedColumns;
    int usedRows;

    bool hoverColorList;
    int hoverIndex;
    Color hoverColor;

    bool widthDragging;
    bool heightDragging;

} ColorBarState;

ColorBarState NewColorBar();
void SetColorBarAnchor(ColorBarState *cb, Vector2 anchor, Vector2 bottom);
// int ColorBar(ColorBarState *state);
bool CurrentColorChanged(ColorBarState *state);

int ColorBarDraw(ColorBarState *state, double dt);
int ColorBarLogic(ColorBarState *state, double dt);
#endif
