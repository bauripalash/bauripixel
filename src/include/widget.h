#ifndef BAURIPIXEL_WIDGET_H
#define BAURIPIXEL_WIDGET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../external/raylib/raylib.h"
#include <stdbool.h>

// Bounds, Size for widgets
typedef struct BpwBounds {
    // Top-Left Anchor
    Vector2 anchor;
    // Bottom-Right Anchor
    Vector2 stopper;
    // Size
    Vector2 size;
} BpwBounds;

// Base Widget
typedef struct BpWidget {
    // widget size
    BpwBounds bounds;
    // parent widget link
    struct BpWidget *parent;
    // should the widget be drawn?
    bool visible;
    // should the widget recieve inputs?
    bool enabled;

    // Update function (self, ctx) -> int
    int (*Update)(struct BpWidget *w, void *ctx);
    // Draw function (self, ctx) -> int
    int (*Draw)(struct BpWidget *w, void *ctx);
} BpWidget;

#define Widget(w) ((BpWidget *)(w))

#ifdef __cplusplus
}
#endif

#endif
