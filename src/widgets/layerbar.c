#include "../include/widgets/layerbar.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/utils.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define LB_INIT_HEIGHT 250
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
    lb.vScrollDragging = false;
    lb.hScrollDragging = false;

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

#define SCROLLT 10
static void updateBounds(LayerBarState *lb) {
    lb->p.bounds.x = lb->anchor.x + LB_MARGIN_LR;
    lb->p.bounds.width = lb->bottom.x - lb->anchor.x - LB_MARGIN_LR * 2;
    lb->p.bounds.y = lb->bottom.y - lb->p.bounds.height - LB_MARGIN_TB * 2;

    Rectangle bounds = lb->p.bounds;

    lb->usableRect =
        (Rectangle){bounds.x + LB_PADDING_LR, bounds.y + LB_PADDING_TB,
                    bounds.width - LB_PADDING_LR * 2 - SCROLLT,
                    bounds.height - LB_PADDING_TB * 2 - SCROLLT};

    lb->toolsRect = (Rectangle){lb->usableRect.x, lb->usableRect.y,
                                lb->usableRect.width, 28};

    lb->layersRect =
        (Rectangle){lb->usableRect.x,
                    lb->toolsRect.y + lb->toolsRect.height + LB_PADDING_TB,
                    lb->toolsRect.width, 0};
    lb->layersRect.height =
        lb->usableRect.height - lb->toolsRect.height - LB_PADDING_TB;

    lb->vScrollRect =
        (Rectangle){lb->layersRect.x + lb->layersRect.width, lb->layersRect.y,
                    SCROLLT, lb->layersRect.height};

    lb->hScrollRect = (Rectangle){lb->layersRect.x + 200,
                                  lb->layersRect.y + lb->layersRect.height,
                                  lb->layersRect.width - 200, SCROLLT};
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

    float btnSize = 25;

    GuiLabelButton(
        (Rectangle){layerToolBounds.x, bounds.y, btnSize, bounds.height},
        GuiIconText(layer->visible ? ICON_EYE_ON : ICON_EYE_OFF, NULL)
    );
    GuiLabelButton(
        (Rectangle){layerToolBounds.x + btnSize, bounds.y, btnSize,
                    bounds.height},
        GuiIconText(ICON_GEAR_BIG, NULL)
    );
    GuiLabelButton(
        (Rectangle){layerToolBounds.x + btnSize * 2, bounds.y, btnSize,
                    bounds.height},
        GuiIconText(ICON_BURGER_MENU, NULL)
    );

    GuiLabelButton(
        (Rectangle){nameBounds.x + btnSize * 3, nameBounds.y,
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

#define MIN_SLIDER 10.0f

void DrawScrollBars(LayerBarState *lb, Rectangle content) {

    Vector2 mpos = GetMousePosition();
    bool hasVerticalScroll = content.height > lb->layersRect.height;
    bool hasHorizontalScroll = content.width > lb->layersRect.width;

    Rectangle viewArea = lb->layersRect;

    DrawRectangleRounded(lb->vScrollRect, 0.9, 0, Fade(ColorBlack, 0.5));
    DrawRectangleRounded(lb->hScrollRect, 0.9, 0, Fade(ColorBlack, 0.5));

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        lb->hScrollDragging = false;
        lb->hScrollDragging = false;
    }

    if (CheckCollisionPointRec(mpos, viewArea)) {
        float wheelMove = GetMouseWheelMove();
        float wheelDelta = wheelMove * 20.0f;
        lb->scroll.y -= wheelDelta;
    }

    float maxScrollY = content.height - viewArea.height;
    if (maxScrollY < 0.0f) {
        maxScrollY = 0.0f;
    }

    float visRatio = viewArea.height / content.height;
    float vSliderH = viewArea.height * visRatio;
    if (vSliderH < 10.0f) {
        vSliderH = 10.0f;
    }

    if (vSliderH < MIN_SLIDER) {
        vSliderH = MIN_SLIDER;
    }

    float vSliderTravel = viewArea.height - vSliderH;
    float vScrollNorm = (maxScrollY > 0.0f) ? lb->scroll.y / maxScrollY : 0.0f;
    float sliderY = lb->vScrollRect.y + vScrollNorm * vSliderTravel;

    Rectangle vThumb = {
        lb->vScrollRect.x,
        sliderY,
        SCROLLT,
        vSliderH,
    };

    Rectangle hThumb = {lb->hScrollRect.x, lb->hScrollRect.y, 20, SCROLLT};

    if (CheckCollisionPointRec(mpos, vThumb) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !lb->vScrollDragging) {
        lb->vScrollDragging = true;
    }

    if (lb->vScrollDragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        float delta = GetMouseDelta().y;
        lb->scroll.y += delta;
    }

    lb->scroll.y = Clamp(lb->scroll.y, 0.0f, maxScrollY);

    if (hasVerticalScroll) {
        DrawRectangleRounded(vThumb, 0.9, 0, ColorVGreen);
    }

    if (hasHorizontalScroll) {
        DrawRectangleRounded(hThumb, 0.9, 0, ColorVGreen);
    }
}

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
            newLayer->name =
                MakeString(TextFormat("Layer %d", newLayer->index));
            AddToLayerList(lb->list, newLayer);
        }
        Rectangle layerContentRect = {
            layersBounds.x, layersBounds.y, layersBounds.width, layersBounds.y
        };

        BpDummyFlatPanel(
            (Rectangle){layersBounds.x - 1, layersBounds.y - 1,
                        layersBounds.width + 4, layersBounds.height + 4},
            2, (Vector4){0}
        );

        float px = layersBounds.x;
        float py = layersBounds.y;
        float pyinc = LAYER_ITEM_HEIGHT - 1;
        // GuiScrollPanel(layersBounds, NULL, layerContentRect, &lb->scroll,
        // &lb->view);

        BeginScissorMode(
            layersBounds.x, layersBounds.y, layersBounds.width,
            layersBounds.height
        );

        for (int i = 0; i < lb->list->count; i++) {
            LayerObj *lr = lb->list->layers[i];
            Rectangle layerBtn = {px, py, 300, LAYER_ITEM_HEIGHT};

            bool isCur = i == lb->curLayer->index;
            if (LayerItemDraw(
                    lb, (Vector2){px - lb->scroll.x, py - lb->scroll.y}, lr,
                    isCur
                )) {
                lb->selLayer = lr;
            }

            py += pyinc;
        }
        EndScissorMode();

        layerContentRect.height = py - layerContentRect.height;
        DrawScrollBars(
            lb, (Rectangle){0, 0, layersBounds.width,
                            (LAYER_ITEM_HEIGHT)*lb->list->count}
        );
    }
    return -1;
}
