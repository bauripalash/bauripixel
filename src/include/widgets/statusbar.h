#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "canvas.h"
#include "colorbar.h"
#include "layerbar.h"
#include "widget.h"
#include <stdbool.h>

// Statusbar Widget object
typedef struct StatusBarState {
	// Widget base prop
    WidgetProp prop;
	// Link to canvas widget state
    CanvasState *canvas;
	// Link to colorbar widget state
    ColorBarState *colorbar;
	// Link to layerbar widget state
    LayerBarState *layerbar;
	// Anchor
    Vector2 anchor;

} StatusBarState;

// Create Menubar state
StatusBarState NewStatusBar();
// Set statusbar position
void SetStatusBarPosition(StatusBarState *state, float x, float height);
// Draw statusbar.
// return not used actually
bool StatusBar(StatusBarState *state, double dt);

#endif
