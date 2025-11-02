#ifndef COLORBAR_H
#define COLORBAR_H

#include "widget.h"
#include <stdbool.h>

// Blank Empty Color
#define CBAR_NOCOLOR          (Color){0, 0, 0, 0}

// Colorbar widget Margin Top, Bottom
#define CBAR_MARGIN_TOPBOTTOM 10.0f
// Colorbar widget Margin Left
#define CBAR_MARGIN_LEFT      10.0f
// Colorbar widget Margin Right
#define CBAR_MARGIN_RIGHT     CBAR_MARGIN_LEFT

// Colorbar Color Box size
#define CBAR_BOX_SIZE         30.0f
// Colorbar Widget Initial Width
#define CBAR_INIT_WIDTH       98.0f
// Colorbar Widget Minimum Width
#define CBAR_MIN_WIDTH        70.0f
// Colorbar Initial Height
#define CBAR_INIT_HEIGHT      300.0f
// Colorbar Resize handle thickness
#define CBAR_HANDLE_THICKNESS 20.0f

#define CB_STATUS_NONE        1
#define CB_STATUS_HDRAG       2
#define CB_STATUS_VDRAG       3

#define ColorsCount(c)        (sizeof(c) / sizeof(Color))

// Colorbar/ Color panel widget state
typedef struct {
	// Widget prop base
    WidgetProp prop;
	// Previous index of color from color palette
    int prevIndex;
	// Current selected index of color from color palette
    int currentIndex;
	// Actual Color object of selected color
    Color currentColor;
	// Top Left anchor of colorbar widget
    Vector2 anchor;
	// Bottom Right anchor of colorbar widget
    Vector2 bottom;

	// Color palette array
    // Will be memory allocated
    // will be used with stb_ds
    Color *colors;
	// Count of colors in color palette
    int colorCount;
	// Scroll vector of Colorbar widget
    Vector2 scroll;
	// View for Raygui's GuiScrollPanel
    Rectangle view;

	// Box size of color box
    int boxSize;
	// Spacing between color boxes
    int boxSpacing;


	// Usable rectangle for GuiScrollPanel
    Rectangle usableRect;
	// Used by color boxes for GuiScrollPanel
    Rectangle usedRect;

	// How many columns of color boxes can be shown
    int maxColumns;
	// How many columns are being used by color boxes
    int usedColumns;
	// How many rows are being used by color boxes
    int usedRows;

	// If Color boxes are being hovered
    bool hoverColorList;
	// Index of hovered color box
    int hoverIndex;
	// Actual color object of hovered color box
    Color hoverColor;

	// If the horizontal resizing handle being dragged
    bool widthDragging;
	// If the vertical resizing handle being dragged
    bool heightDragging;

} ColorBarState;

// Create new Colorbar Widget state
ColorBarState NewColorBar();
// Set Anchors for Colorbar widget
void SetColorBarAnchor(ColorBarState *cb, Vector2 anchor, Vector2 bottom);
// Returns true if Current selected color has been changed
bool CurrentColorChanged(ColorBarState *state);

// Draw Colorbar widget
int ColorBarDraw(ColorBarState *state, double dt);
// Colorbar widget logic
int ColorBarLogic(ColorBarState *state, double dt);
#endif
