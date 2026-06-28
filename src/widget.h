/*
 * widget.c - Widget Definitions
 * Copyright (C) <year>  <name of author>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BAURIPIXEL_WIDGET_H
#define BAURIPIXEL_WIDGET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "external/raylib/raylib.h"
#include <stdbool.h>

/*
 * Margin And Padding Infos
 *
 */

// Color Panel Margin
#define CLRPANEL_MARGIN 5
// Color Panel Padding
#define CLRPANEL_PADDING 5

// Draw Area Margin
#define DRWAREA_MARGIN 10
// Draw Area Padding
#define DRWAREA_PADDING 10

// Base Widget
typedef struct BpWidget {
    // widget bound
    Rectangle bounds;
    // widget top-left anchor
    Vector2 anchor;
    // widget bottom-right stopper
    Vector2 stopper;
    // parent widget link
    struct BpWidget *parent;
    // should the widget be drawn?
    bool visible;
    // should the widget recieve inputs?
    bool enabled;

    // Update function (self, ctx) -> int
    int (*Update)(struct BpWidget *w, double dt, void *ctx);
    // Draw function (self, ctx) -> int
    int (*Draw)(struct BpWidget *w, double dt, void *ctx);
} BpWidget;

// Caste any widget to base Widget
#define Widget(w) ((BpWidget *)(w))

// Color Selector Panel
typedef struct BpColorPanel {
    BpWidget w;
    // Color Palette array
    Color *colors;
    // Color Palette count
    int colorCount;
    // Current selected color index from Color Palette (starts with 0)
    int curColorIndex;
    // Current selected color
    Color curColor;
    // Hovering color index from Color Palette
    int hoverColorIndex;
    // Hovering color
    Color hoverColor;

    // How much size each colored boxes take
    int boxSize;
    // How much space we can use from drawing colored boxes
    Rectangle usableRect;
    // How much space we actually used based on color count
    Rectangle usedRect;
    // How many numbers of columns of colored boxes can be shown
    int maxColumns;
    // How many numbers of rows will actually be used.
    int usedRow;
    // How many numbers of column will actually be used.
    int usedColumn;

    // Scroll Vector for Raygui Scroll panel
    Vector2 scroll;
    // View Rectangle for Raygui Scroll panel
    Rectangle view;

} BpColorPanel;

// Create New Color Panel Widget
BpColorPanel NewColorPanel(void);

// DrawArea Widget
typedef struct BpDrawArea {
    BpWidget w;
    // Current selected color
    Color color;
    // The actual canvas width in pixels
    int canvasWidth;
    // The actual canvas height in pixels
    int canvasHeight;

    // Selected background color
    Color bgColor;

    // The checkerd or solid background image as the base for drawing
    Image bgImg;
    // Texture for background image
    Texture2D bgTxt;

    // Zoom and Panning controller camera
    Camera2D camera;

    // The virtual `Player` for controlling the drawing canvas
    Vector2 point;

    // Is the mouse inside the usable rectangle
    bool hovering;
    // Is the canvas being panned/moved
    bool panning;
    // Usable Rectangle where the canvas can move within, the viewable area
    Rectangle viewport;
    Rectangle canvasRect;

} BpDrawArea;

// Create new DrawArea
// Parameters:
// canvasW => Width of the actual drawing in pixels
// canvasH => Height of the actual drawing in pixels
BpDrawArea NewDrawArea(int canvasW, int canvasH);

#ifdef __cplusplus
}
#endif

#endif
