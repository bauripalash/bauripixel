#ifndef DRAWTOOLBAR_H
#define DRAWTOOLBAR_H

#include "../../external/raylib.h"
#include "../drawtools.h"
#include "widget.h"
#include <stdbool.h>

// Default Drawtool button size
#define DEFAULT_DT_BTN_SIZE 40
// Default DrawToolBar widget width
#define DEFAULT_DTBAR_WIDTH 50
// Default DrawToolBar Left Margin
#define DTBAR_MARGIN_L      8
// Default DrawToolBar Right Margin
#define DTBAR_MARGIN_R      DTBAR_MARGIN_L
// Default DrawToolBar Top Down Margin
#define DTBAR_MARGIN_TB     10

// DrawToolBar Widget State
typedef struct {
	// Widget prop base
    WidgetProp prop;
	// Widget Top Left Anchor
    Vector2 anchor;
	// Widget Bottom Right Anchor
    Vector2 bottom;

	// DrawToolBar Options Rectangle Bounds
    Rectangle optRect;
	// DrawToolBar Buttons Rectangle Bounds
    Rectangle toolsRect;

	// DrawToolBar Button Margin
    Vector2 toolBtnMargin;
	// DrawToolBar Button Padding
    Vector2 toolbarPadding;

	// Current Selected Tool
    DrawTool currentTool;

	// Current Brush Size
    int brushSize;
	// Maximum Brush size
    int maxBrushSize;
	// Font (cache for easy access)
    Font font;

	// Brush size popup slider is being hovered
    bool sliderHover;
	// Selected brush shape
    BrushShape brushShape;

	// Is brush size being edited
    bool brushSizeEdit;

	// Drawtool button icons in a single image
    Image iconsImg;
	// DrawToolBar button icon texture
    Texture2D iconsTxt;

} DrawToolBarState;

// As said handles shortcuts and returns newly selected drawtool
DrawTool HandleDToolsShortcuts(DrawToolBarState *state);
// Create new DrawToolBar state
DrawToolBarState NewDrawToolBar();
// Set DrawToolBar anchors
void SetDrawToolBarAnchor(
    DrawToolBarState *dtb, Vector2 anchor, Vector2 bottom
);
// Free DrawToolBar state
void FreeDrawToolBar(DrawToolBarState *state);
// Draw the DrawToolBar
// return => not used
int DrawToolbar(DrawToolBarState *state, double dt);

#endif
