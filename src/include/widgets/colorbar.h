#ifndef COLORBAR_H
#define COLORBAR_H

#include "widget.h"
#include <stdbool.h>

typedef struct {
    WidgetProp prop;
    Color currentColor;
    int colorCount;
    Vector2 scroll;
} ColorBarState;

ColorBarState NewColorBar();
bool ColorBar(ColorBarState *state);
#endif
