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

#define Widget(w) ((BpWidget *)(w))

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

BpColorPanel NewColorPanel(void);

typedef struct BpDrawArea {
    BpWidget w;
    Color color;

    Color bgColor;
    Image bgImg;
    Texture2D bgTxt;

    Rectangle usableRect;

} BpDrawArea;

BpDrawArea NewDrawArea(void);

#ifdef __cplusplus
}
#endif

#endif
