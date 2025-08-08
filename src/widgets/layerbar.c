#include "../include/widgets/layerbar.h"
#include "../include/colors.h"
#include "../include/components.h"
#include <stdbool.h>
#include <stdlib.h>

#define LB_INIT_HEIGHT 150
#define LB_MIN_HEIGHT  50
#define LB_MARGIN_LR   10
#define LB_MARGIN_TB   10
#define LB_PADDING_LR  10
#define LB_PADDING_TB  10

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

    lb.usableRect = (Rectangle){0};
    lb.toolsRect = (Rectangle){0};
    lb.layersRect = (Rectangle){0};

    return lb;
}

void FreeLayerBar(LayerBarState *lb) { return; }

static void updateBounds(LayerBarState *lb) {
    lb->p.bounds.x = lb->anchor.x + LB_MARGIN_LR;
    lb->p.bounds.width = lb->bottom.x - lb->anchor.x - LB_MARGIN_LR * 2;
    lb->p.bounds.y = lb->bottom.y - lb->p.bounds.height - LB_MARGIN_TB * 2;

    Rectangle bounds = lb->p.bounds;

    lb->usableRect =
        (Rectangle){bounds.x + LB_PADDING_LR, bounds.y + LB_PADDING_TB,
                    bounds.width - LB_PADDING_LR * 2,
                    bounds.height - LB_PADDING_TB * 2};

    lb->toolsRect = (Rectangle){lb->usableRect.x, lb->usableRect.y,
                                lb->usableRect.width, 28};

    lb->layersRect =
        (Rectangle){lb->usableRect.x,
                    lb->toolsRect.y + lb->toolsRect.height + LB_PADDING_TB,
                    lb->toolsRect.width, 0};
    lb->layersRect.height =
        lb->usableRect.height - lb->toolsRect.height - LB_PADDING_TB;
}

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

    updateBounds(lb);
}

#define HANDLE_THICKNESS  10

#define LAYER_ITEM_HEIGHT 35

#define LAYER_NAME_WIDTH  200

bool LayerItemDraw(
    LayerBarState *lb, Vector2 pos, LayerObj *layer, bool isCur
) {
    Font f = GuiGetFont();
    bool locked = GuiIsLocked();
    int fontSize = f.baseSize;

    Color bg = ColorGrayDarker;
    Color normalBrdr = ColorGrayLightest;
    Color hoverBrdr = ColorGrayLighter;
    Color activeBrdr = ColorWhite;
    Color brdr = normalBrdr;
    Vector2 mpos = GetMousePosition();

    Rectangle bounds = {pos.x, pos.y, lb->layersRect.width, LAYER_ITEM_HEIGHT};

    bool hover = CheckCollisionPointRec(mpos, bounds) && !locked;
    bool clicked = hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (hover) {
        brdr = hoverBrdr;
    }

    if (isCur) {
        brdr = activeBrdr;
    }

    Rectangle layerToolBounds = {
        bounds.x + 5, bounds.y, bounds.height * 3, bounds.height
    };
    Rectangle nameBounds = {
        bounds.x, bounds.y, LAYER_NAME_WIDTH, bounds.height
    };
    BpDummyFlatPanel(bounds, 2, (Vector4){0});
    BpDummyFlatPanel(nameBounds, 2, (Vector4){});
    GuiLabelButton(
        (Rectangle){layerToolBounds.x, bounds.y, bounds.height, bounds.height},
        GuiIconText(layer->visible ? ICON_EYE_ON : ICON_EYE_OFF, NULL)
    );
    GuiLabelButton(
        (Rectangle){layerToolBounds.x + bounds.height, bounds.y, bounds.height,
                    bounds.height},
        GuiIconText(ICON_GEAR_BIG, NULL)
    );
    GuiLabelButton(
        (Rectangle){layerToolBounds.x + bounds.height + bounds.height, bounds.y,
                    bounds.height, bounds.height},
        GuiIconText(ICON_BURGER_MENU, NULL)
    );

    GuiLabelButton(
        (Rectangle){nameBounds.x + layerToolBounds.width, nameBounds.y,
                    nameBounds.width - layerToolBounds.width,
                    nameBounds.height},
        layer->name
    );
    // GuiLabelButton(nameBounds, layer->name);

    // DrawRectangleRounded(bounds, 0.125, 0, bg);
    // DrawRectangleRoundedLinesEx(rect, 0.125, 0, 2, brdr);

    /*
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
    */

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
        Rectangle usableBounds = lb->usableRect;

        Rectangle toolBarBounds = lb->toolsRect;

        Rectangle layersBounds = lb->layersRect;

        BpRoundedPanel(bounds, 2, 0.125, true);

        if (BpDummyButton((Rectangle){toolBarBounds.x, toolBarBounds.y,
                                      toolBarBounds.height,
                                      toolBarBounds.height})) {
            LayerObj *newLayer = NewLayerObj(lb->gridSize.x, lb->gridSize.y);
            newLayer->index = lb->list->count;
            AddToLayerList(lb->list, newLayer);
        }

        float px = layersBounds.x;
        float py = layersBounds.y;
        float pyinc = LAYER_ITEM_HEIGHT - 1;

        for (int i = 0; i < lb->list->count; i++) {
            LayerObj *lr = lb->list->layers[i];
            Rectangle layerBtn = {px, py, 300, LH};

            bool isCur = i == lb->curLayer->index;
            if (LayerItemDraw(lb, (Vector2){px, py}, lr, isCur)) {
                lb->selLayer = lr;
            }

            py += pyinc;
        }
    }
    return -1;
}
