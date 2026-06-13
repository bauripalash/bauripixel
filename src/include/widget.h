#ifndef BAURIPIXEL_WIDGET_H
#define BAURIPIXEL_WIDGET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../external/raylib/raylib.h"
#include <stdbool.h>

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
    // From Color Palette (starts with 0)
    int colorIndex;

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

} BpColorPanel;

BpColorPanel NewColorPanel(void);

#ifdef __cplusplus
}
#endif

#endif
