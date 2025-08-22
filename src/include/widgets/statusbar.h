#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "canvas.h"
#include "colorbar.h"
#include "layerbar.h"
#include "widget.h"
#include <stdbool.h>
typedef struct StatusBarState {

    WidgetProp prop;
    CanvasState *canvas;
    ColorBarState *colorbar;
    LayerBarState *layerbar;
    Vector2 anchor;

} StatusBarState;

StatusBarState NewStatusBar();
void SetStatusBarPosition(StatusBarState *state, float x, float height);
bool StatusBar(StatusBarState *state);

#endif
