#include "../external/raylib/raylib.h"
#include "../external/stb/stb_ds.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/widget.h"
#include "raylib.h"
#include <stdbool.h>

#define CP_MARGIN_TB 5

static void updateBounds(BpColorPanel *panel) {
    panel->w.bounds.x = CP_MARGIN_TB + panel->w.anchor.x;
    panel->w.bounds.y = CP_MARGIN_TB + panel->w.anchor.y;
}

int colorPanelDraw(BpWidget *base, double dt, void *ctx) {
    TraceLog(
        LOG_WARNING, "bounds -> %f, %f, %f, %f", base->bounds.x, base->bounds.y,
        base->bounds.width, base->bounds.height
    );
    BpPanelBorder(base->bounds, 5);
    return 0;
}

int colorPanelUpdate(BpWidget *base, double dt, void *ctx) {
    updateBounds((BpColorPanel *)base);
    return 0;
}

static void pushDefaultColors(BpColorPanel *panel) {
    arrput(panel->colors, BpColorVGray);
    arrput(panel->colors, BpColorVGrayLight);
    arrput(panel->colors, BpColorVGreen);
    arrput(panel->colors, BpColorVWhite);
    panel->colorCount = 4;
}

BpColorPanel NewColorPanel(void) {
    BpColorPanel cp = {0};
    cp.w = (BpWidget){
        .bounds = (Rectangle){0, 0, 80, 200},
        .anchor = (Vector2){0},
        .stopper = (Vector2){0},
        .enabled = true,
        .visible = true,
        .parent = NULL,
        .Update = colorPanelUpdate,
        .Draw = colorPanelDraw,
    };
    cp.colorIndex = 0;
    cp.boxSize = 50;
    pushDefaultColors(&cp);

    return cp;
}
