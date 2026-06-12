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
    int colorIndex;
    int boxSize;
    Color *colors;
    int colorCount;
} BpColorPanel;

BpColorPanel NewColorPanel(void);

#ifdef __cplusplus
}
#endif

#endif
