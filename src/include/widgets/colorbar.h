#ifndef COLORBAR_H
#define COLORBAR_H

#include "widget.h"
#include <stdbool.h>

#define NOCOLOR           (Color){0, 0, 0, 0}
#define DEFAULT_BOX_SIZE  30
#define DEFAULT_BOX_SPACE 0
#define ColorsCount(c)    (sizeof(c) / sizeof(Color))
#define PANEL_PADDING     5
#define MARGIN_TOPBOTTOM  10
#define MARGIN_LEFT       10
#define INIT_WIDTH        80.0f
#define HANDLE_THICKNESS  20

typedef struct {
    WidgetProp prop;
    int currentIndex;
    Color currentColor;
    Vector2 anchor;
    Vector2 bottomStop;

    // Will be memory allocated
    // will be used with stb_ds
    Color *colors;
    int colorCount;
    Vector2 scroll;
    int boxSize;
    int boxSpacing;
    Color cbBg;
    Color cbBorder;

    Rectangle content;
    Rectangle view;
    bool widthDragging;
    bool heightDragging;

    int scrollBarWidth; // should be filled after theme stuff

} ColorBarState;

ColorBarState NewColorBar();
void SetColorBarAnchor(ColorBarState *cb, Vector2 anchor, Vector2 bottom);
bool ColorBar(ColorBarState *state);
int AddToColorBar(ColorBarState *cb, Color color);
void ClearColorBar(ColorBarState *cb);
#endif
