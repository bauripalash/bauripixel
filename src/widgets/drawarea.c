#include "../colors.h"
#include "../components.h"
#include "../external/raylib/raylib.h"
#include "../widget.h"
#include "raylib.h"

int drawAreaDraw(BpWidget *base, double dt, void *ctx) {
    if (!base->enabled || !base->visible) {
        return 1;
    }

    BpDrawArea *da = (BpDrawArea *)base;
    BpPanelBorder(base->bounds, 2);
    DrawRectangleLinesEx(da->usableRect, 2, BpColorBlack);

    return 0;
}

int drawAreaUpdate(BpWidget *base, double dt, void *ctx) {
    if (!base->enabled) {
        return 1;
    }
    BpDrawArea *da = (BpDrawArea *)base;
    Rectangle bounds = base->bounds;
    Rectangle usableRect = {
        bounds.x + DRWAREA_PADDING, bounds.y + DRWAREA_PADDING,
        bounds.width - DRWAREA_PADDING * 2, bounds.height - DRWAREA_PADDING * 2
    };

    da->usableRect = usableRect;

    return 0;
}

BpDrawArea NewDrawArea(void) {
    BpDrawArea da = {0};
    da.w = (BpWidget){
        .enabled = true,
        .visible = true,
        .bounds = (Rectangle){10, 10, 400, 400},
        .anchor = (Vector2){0},
        .stopper = (Vector2){0},
        .Update = drawAreaUpdate,
        .Draw = drawAreaDraw
    };
    da.color = BpColorBlack;
    // da.bgImg = GenImageChecked(, int height, int checksX, int checksY, Color
    // col1, Color col2)
    return da;
}
