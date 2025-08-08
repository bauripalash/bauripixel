#include "../include/widgets/layerbar.h"
#include "../include/colors.h"
#include "../include/components.h"
#include <stdbool.h>
#include <stdlib.h>

#define LB_INIT_HEIGHT 150
#define LB_MIN_HEIGHT  50
#define LB_MARGIN_LR   10
#define LB_MARGIN_TB   10

LayerBarState NewLayerBar() {
    LayerBarState lb = {0};
    lb.p = NewWidgetProp();
    lb.p.bounds.height = LB_INIT_HEIGHT;
    lb.anchor = (Vector2){0};
    lb.bottom = (Vector2){0};
    lb.gridSize = (Vector2){0};

    lb.preview = true;
    lb.list = NULL;
    lb.curLayer = NULL;
    lb.selLayer = NULL;

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
    lb->p.bounds.width = lb->bottom.x - lb->anchor.x - LB_MARGIN_LR * 2;
    lb->p.bounds.y = lb->bottom.y - lb->p.bounds.height - LB_MARGIN_TB * 2;
}

#define HANDLE_THICKNESS 10

bool LayerItemDraw(
    LayerBarState *lb, Rectangle rect, LayerObj *layer, bool isCur
) {
    Font f = GuiGetFont();
    bool locked = GuiIsLocked();

    Color bg = ColorGrayDarker;
    Color normalBrdr = ColorGrayLightest;
    Color hoverBrdr = ColorGrayLighter;
    Color activeBrdr = ColorWhite;
    Color brdr = normalBrdr;

    Vector2 mpos = GetMousePosition();

    bool hover = CheckCollisionPointRec(mpos, rect) && !locked;
    bool clicked = hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (hover) {
        brdr = hoverBrdr;
    }

    if (isCur) {
        brdr = activeBrdr;
    }

    DrawRectangleRounded(rect, 0.125, 0, bg);
    DrawRectangleRoundedLinesEx(rect, 0.125, 0, 2, brdr);

    Rectangle previewRect = {
        rect.x + 5, rect.y + 5, rect.height - 10, rect.height - 10
    };

    Rectangle prevSrc = {
        0,
        0,
        layer->img.width,
        layer->img.height,
    };

    Vector2 origin = {0};

    DrawTexturePro(*lb->previewBg, prevSrc, previewRect, origin, 0, ColorWhite);
    DrawTexturePro(layer->txt, prevSrc, previewRect, origin, 0, ColorWhite);

    Vector2 textpos = {
        rect.x + 5 + previewRect.width + 5,
        rect.y + (rect.height / 2.0f) - f.baseSize / 2.0f
    };

    GuiDrawIcon(
        layer->visible ? ICON_EYE_ON : ICON_EYE_OFF, textpos.x, textpos.y, 1,
        ColorWhite
    );
    textpos.x += 16;

    DrawTextEx(
        f, TextFormat("Layer #%d", layer->index), textpos, f.baseSize, 0,
        ColorWhite
    );

    return clicked;
}

int LayerBarLogic(LayerBarState *lb) {
    if (lb->p.active) {
        updateBounds(lb);
        bool locked = GuiIsLocked();

        Rectangle bounds = lb->p.bounds;
        Rectangle handleRect = {
            bounds.x, bounds.y - (HANDLE_THICKNESS / 2.0f), bounds.width,
            HANDLE_THICKNESS
        };

        Vector2 mpos = GetMousePosition();

        bool atBounds = CheckCollisionPointRec(mpos, bounds);
        bool atHandle = CheckCollisionPointRec(mpos, handleRect);

        if (atHandle && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !locked) {
            lb->resizeDragging = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !locked) {
            lb->resizeDragging = false;
        }

        if (lb->resizeDragging) {
            Vector2 delta = GetMouseDelta();
            lb->p.bounds.height -= delta.y;
        }

        if (lb->p.bounds.height < LB_MIN_HEIGHT) {
            lb->p.bounds.height = LB_MIN_HEIGHT;
        }
    }

    return -1;
}

#define LH 40

int LayerBarDraw(LayerBarState *lb) {
    if (lb->p.active) {
        bool locked = GuiIsLocked();

        Rectangle bounds = lb->p.bounds;

        BpRoundedPanel(bounds, 2, 0.125, true);

        if (BpDummyButton((Rectangle){bounds.x + 10, bounds.y + 5, 32, 32})) {
            LayerObj *newLayer = NewLayerObj(lb->gridSize.x, lb->gridSize.y);
            newLayer->index = lb->list->count;
            AddToLayerList(lb->list, newLayer);
        }

        float px = lb->p.bounds.x + 10;
        float py = bounds.y + 32 + 10;
        float pyinc = LH + 10;

        for (int i = 0; i < lb->list->count; i++) {
            LayerObj *lr = lb->list->layers[i];
            Rectangle layerBtn = {px, py, 300, LH};

            bool isCur = i == lb->curLayer->index;
            if (LayerItemDraw(lb, layerBtn, lr, isCur)) {
                lb->selLayer = lr;
            }

            py += pyinc;
        }
    }
    return -1;
}
