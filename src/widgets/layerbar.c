#include "../include/widgets/layerbar.h"
#include "../include/colors.h"
#include "../include/components.h"
#include <stdbool.h>
#include <stdlib.h>

#define LB_INIT_HEIGHT 100
#define LB_MARGIN_LR   10
#define LB_MARGIN_TB   10

LayerBarState NewLayerBar() {
    LayerBarState lb = {0};
    lb.p = NewWidgetProp();
    lb.p.bounds.height = LB_INIT_HEIGHT;
    lb.anchor = (Vector2){0};
    lb.bottom = (Vector2){0};

    lb.preview = true;
    lb.list = NULL;

    lb.scroll = (Vector2){0};
    lb.view = (Rectangle){0};
    lb.resizeDragging = false;

    lb.p.bounds.x = lb.anchor.x + LB_MARGIN_LR;
    lb.p.bounds.width = GetScreenWidth() - LB_MARGIN_LR * 2;
    lb.p.bounds.y =
        GetScreenHeight() - lb.p.bounds.height - lb.bottom.y - LB_MARGIN_TB;
    return lb;
}

void FreeLayerBar(LayerBarState *lb) { return; }

void SetLayerBarAnchor(LayerBarState *lb, Vector2 anchor, Vector2 bottom) {
    if (anchor.x != -1) {
        lb->anchor.x = anchor.x;
    }

    if (anchor.y != -1) {
        lb->anchor.y = anchor.y;
    }

    if (bottom.x != -1) {
        lb->bottom.x = bottom.x;
    }

    if (bottom.y != -1) {
        lb->bottom.y = bottom.y;
    }
}

static void updateBounds(LayerBarState *lb) {
    lb->p.bounds.x = lb->anchor.x + LB_MARGIN_LR;
    lb->p.bounds.width = GetScreenWidth() - LB_MARGIN_LR * 2;
    lb->p.bounds.y =
        GetScreenHeight() - lb->p.bounds.height - lb->bottom.y - LB_MARGIN_TB;
}

int LayerBarLogic(LayerBarState *lb) {
    if (lb->p.active) {
        updateBounds(lb);
    }

    return -1;
}

#define LH 16

int LayerBarDraw(LayerBarState *lb) {
    if (lb->p.active) {
        BpRoundedPanel(lb->p.bounds, 0.125);

        float px = lb->p.bounds.x + 20;
        float py = lb->p.bounds.y + 20;

        for (int i = 0; i < lb->list->count; i++) {
            GuiLabelButton(
                (Rectangle){px, py + (i * LH), 100, 30},
                TextFormat("Layer #%d", i)
            );
        }
    }
    return -1;
}
