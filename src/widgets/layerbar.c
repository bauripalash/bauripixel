#include "../include/widgets/layerbar.h"
#include "../external/raymath.h"
#include "../external/stb/stb_ds.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/options.h"
#include "../include/utils.h"
#include <stdbool.h>
#include <stdlib.h>

#define LB_INIT_HEIGHT 120
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
    lb.selectedLayer = NULL;

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
    lb.wLayerOpts = NewWLayerOpts();

    lb.menuSelLayer = NULL;
    lb.menuOpen = false;
    lb.menuPos = Vector2Zero();

    lb.draggingLayer = false;
    lb.dragLayer = NULL;

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

static bool createNewLayer(LayerBarState *lb, LayerObj *targetLayer) {
    LayerObj *newLayer =
        NewLayerObj(lb->gridSize.x, lb->gridSize.y, lb->curLayer->flist->count);
    if (newLayer == NULL) {
        return false;
    }

    newLayer->index = lb->list->count;
    newLayer->name = MakeString(TextFormat("Layer %d", newLayer->index));

    AddToLayerList(
        lb->list, newLayer
    ); // here the new layer will be added to the end

    if (targetLayer != NULL) {
        MoveIdxLayerList(lb->list, newLayer->index, targetLayer->index + 1);
    }

    return true;
}

static bool createNewFrame(LayerBarState *lb) {
    for (int i = 0; i < lb->list->count; i++) {
        LayerObj *lr = lb->list->layers[i];
        if (lr == NULL) {
            return false;
        }

        FrameObj *frame = NewFrameObj(lr->img.width, lr->img.height);
        if (frame == NULL) {
            return false;
        }

        AddToFrameList(lr->flist, frame);
    }
    return true;
}

static int curFrame = 0;
static bool selFrameAll = false;

#define HANDLE_THICKNESS  20
#define LAYER_ITEM_HEIGHT 35
#define LAYER_NAME_WIDTH  200
#define LAYER_ITEM_MARGIN 5

bool LayerItemFrameDraw(
    LayerBarState *lb, Vector2 pos, LayerObj *layer, bool isCur
) {
    int framecount = layer->flist->count;
    Rectangle cellRect = {pos.x, pos.y, LAYER_ITEM_HEIGHT, LAYER_ITEM_HEIGHT};

    for (int i = 0; i < framecount; i++) {
        FrameObj *f = layer->flist->frames[i];
        BpFramePrevBox(cellRect, f, false);
        cellRect.x += cellRect.width;
    }

    return true;
}

bool LayerItemDraw(
    LayerBarState *lb, Vector2 pos, LayerObj *layer, bool isCur
) {
    int ogBorder = OptThemeGetSet(T_PANEL_BORDER, OptThemeGet(T_LAYER_BRDR));
    bool locked = GuiIsLocked();

    Vector2 mpos = GetMousePosition();

    Rectangle bounds = {
        pos.x, pos.y, lb->layersRect.width - LAYER_ITEM_MARGIN * 2,
        LAYER_ITEM_HEIGHT
    };

    bool hover = CheckCollisionPointRec(mpos, bounds) && !locked;
    bool clicked = hover && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
    float halfHeight = bounds.height / 2.0f;

    Rectangle layerToolBounds = {
        bounds.x + 5, bounds.y, bounds.height * 3, bounds.height
    };
    Rectangle nameBounds = {
        bounds.x, bounds.y, LAYER_NAME_WIDTH, bounds.height
    };

    BpPanelBorder(nameBounds, 2);

    if (hover && (lb->draggingLayer)) {
        lb->dragTarget = layer->index;
        Rectangle upperBounds = {bounds.x, bounds.y, bounds.width, halfHeight};
        if (CheckCollisionPointRec(mpos, upperBounds)) {
            lb->putDragAtEnd = false;
        } else {
            lb->putDragAtEnd = true;
        }
    }

    if (hover && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        lb->menuSelLayer = layer;
    }

    float btnSize = 25;
    Rectangle optsBtn = {
        layerToolBounds.x, layerToolBounds.y, btnSize, layerToolBounds.height
    };
    float optsInc = btnSize + 2;

    bool visibility =
        GuiLabelButton(
            optsBtn,
            GuiIconText(layer->visible ? ICON_EYE_ON : ICON_EYE_OFF, NULL)
        ) != 0;

    if (visibility) {
        layer->visible = !layer->visible;
    }

    optsBtn.x += optsInc;

    bool lock = GuiLabelButton(optsBtn, GuiIconText(ICON_LOCK_OPEN, NULL)) != 0;
    optsBtn.x += optsInc;
    Rectangle layerNameRect = {
        optsBtn.x, optsBtn.y, nameBounds.width - (optsInc * 2),
        nameBounds.height
    };

    GuiLabel(layerNameRect, TextFormat("#%d %s", layer->index, layer->name));

    if (CheckCollisionPointRec(mpos, layerNameRect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !locked) {
        lb->draggingLayer = true;
        lb->dragLayer = layer;
    }

    LayerItemFrameDraw(
        lb, (Vector2){pos.x + LAYER_NAME_WIDTH, pos.y}, layer, isCur
    );

    OptThemeSet(T_PANEL_BORDER, ogBorder);

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

#define MENU_HEIGHT_LAYER  120
#define MENU_HEIGHT_GLOBAL 200
#define MENU_WIDTH         150
#define MENU_ITEM_HEIGHT   18
#define MENU_PADDING       5

typedef enum CtxMenuResult {
    CMR_NONE = 0,
    CMR_CLOSE = 1,
} CtxMenuResult;

CtxMenuResult CtxMenu(LayerBarState *lb) {
    CtxMenuResult result = CMR_NONE;
    bool locked = GuiIsLocked();
    Vector2 mpos = GetMousePosition();
    bool hasLayer = lb->menuSelLayer != NULL;

    float panelHeight = hasLayer ? MENU_HEIGHT_LAYER : MENU_HEIGHT_GLOBAL;

    Rectangle rect = {lb->menuPos.x, lb->menuPos.y, MENU_WIDTH, panelHeight};

    float bottomPos = rect.y + rect.height;
    float screenH = GetScreenHeight();
    if (bottomPos >= screenH) {
        float diff = bottomPos - screenH;
        rect.y -= diff + LB_MARGIN_TB;
        lb->menuPos.y = rect.y;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        !CheckCollisionPointRec(mpos, rect) && !locked) {
        result = CMR_CLOSE;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && lb->menuOpen &&
        !CheckCollisionPointRec(mpos, rect) && !locked) {
        result = CMR_CLOSE;
    }

    BpPanelBorder(rect, 2);
    Rectangle labelRect = {
        rect.x + MENU_PADDING, rect.y + MENU_PADDING,
        rect.width - MENU_PADDING * 2, MENU_ITEM_HEIGHT
    };
    float yInc = MENU_ITEM_HEIGHT + MENU_PADDING;

    if (lb->menuSelLayer != NULL) {
        if (BpLabelButton(labelRect, "Properties")) {
            SetupWLayerOpts(&lb->wLayerOpts, lb->menuSelLayer);
            lb->wLayerOpts.p.active = true;
            result = CMR_CLOSE;
        }

        labelRect.y += yInc;

        DrawLineEx(
            (Vector2){rect.x, labelRect.y},
            (Vector2){rect.x + rect.width, labelRect.y}, 2,
            GetColor(OptThemeGet(T_PANEL_BORDER))
        );
        labelRect.y += MENU_PADDING;

        if (BpLabelButton(labelRect, "New Layer")) {
            // Create a new layer and push it to index after the selLayer
            createNewLayer(lb, lb->menuSelLayer);
            result = CMR_CLOSE;
        }

        labelRect.y += yInc;
        if (BpLabelButton(labelRect, "Duplicate Layer")) {
            // Duplicate and push it index after
            DuplicateIdxLayerList(lb->list, lb->menuSelLayer->index);
            lb->curLayer =
                GetLayerFromList(lb->list, lb->menuSelLayer->index + 1);
            result = CMR_CLOSE;
        }
        labelRect.y += yInc;
        if (BpLabelButton(labelRect, "Delete Layer")) {
            LayerObj *lr =
                RemoveIdxLayerList(lb->list, lb->menuSelLayer->index);
            TraceLog(LOG_ERROR, "count -> %d", lb->list->count);
            FreeLayerObj(lr);
            result = CMR_CLOSE;
        }
    }
    return result;
}

int LayerBarDraw(LayerBarState *lb) {
    if (lb->p.active) {
        lb->anypopup = lb->wLayerOpts.p.active;
        bool locked = GuiIsLocked();
        Vector2 mpos = GetMousePosition();

        Rectangle bounds = lb->p.bounds;

        Rectangle toolBarBounds = lb->toolsRect;

        Rectangle layersBounds = lb->layersRect;

        // BpRoundedPanel(bounds, 2, 0.125, true);
        BpPanelBorder(bounds, 2);
        Rectangle toolBtnRect = {
            toolBarBounds.x, toolBarBounds.y, toolBarBounds.height,
            toolBarBounds.height
        };

        if (BpTextButton(toolBtnRect, GuiIconText(ICON_TARGET_SMALL, NULL))) {
            createNewLayer(lb, lb->curLayer);
        }
        toolBtnRect.x += toolBtnRect.width;
        if (BpTextButton(toolBtnRect, GuiIconText(ICON_PLAYER_JUMP, NULL))) {
            createNewFrame(lb);
        }

        Rectangle layerContentRect = {
            layersBounds.x, layersBounds.y, layersBounds.width, layersBounds.y
        };

        BpPanelBorder(
            (Rectangle){layersBounds.x - 1, layersBounds.y - 1,
                        layersBounds.width + 4, layersBounds.height + 4},
            2
        );

        float px = layersBounds.x + LAYER_ITEM_MARGIN;
        float py = layersBounds.y + LAYER_ITEM_MARGIN;
        float pyinc = LAYER_ITEM_HEIGHT - 1;

        BeginScissorMode(
            layersBounds.x, layersBounds.y, layersBounds.width,
            layersBounds.height
        );

        Rectangle activeRect = {0};
        for (int i = 0; i < lb->list->count; i++) {
            LayerObj *lr = lb->list->layers[i];
            Rectangle layerBtn = {px, py, 300, LAYER_ITEM_HEIGHT};
            float posX = px - lb->scroll.x;
            float posY = py - lb->scroll.y;

            bool isCur = i == lb->curLayer->index;
            if (LayerItemDraw(lb, (Vector2){posX, posY}, lr, isCur)) {
                lb->selectedLayer = lr;
            }

            if (isCur) {
                activeRect =
                    (Rectangle){posX, posY,
                                lb->layersRect.width - LAYER_ITEM_MARGIN * 2,
                                LAYER_ITEM_HEIGHT};
            }

            // Drawing the drag mark
            if (lb->dragTarget == lr->index && lb->draggingLayer &&
                lb->dragTarget != lb->curLayer->index) {
                float ypos = posY;

                if (lb->putDragAtEnd) {
                    ypos += LAYER_ITEM_HEIGHT - 8;
                }

                DrawRectangleRec(
                    (Rectangle){posX, ypos,
                                lb->layersRect.width - LAYER_ITEM_MARGIN * 2,
                                8},
                    BpColorSetAlpha(ColorVGreen, 200)
                );
            }

            py += pyinc;
        }
        int ogBorder =
            OptThemeGetSet(T_PANEL_BORDER, OptThemeGet(T_LAYER_ACTIVE_BRDR));
        BpPanelOnlyBorder(activeRect, 2);

        /*
if (!selFrameAll) {
    Rectangle frameRect = {
        activeRect.x + LAYER_NAME_WIDTH +
            (curFrame * activeRect.height),
        activeRect.y, activeRect.height, activeRect.height
    };
    BpFramePrevActive(frameRect, NULL, false);
}
        */

        OptThemeSet(T_PANEL_BORDER, ogBorder);

        EndScissorMode();

        if (lb->draggingLayer && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            int fromIndex = lb->dragLayer->index;
            int toIndex = lb->dragTarget;
            if (fromIndex != toIndex) {
                if (lb->putDragAtEnd) {
                    toIndex++;
                }

                toIndex = ClampInt(toIndex, 0, lb->list->count - 1);

                if (lb->putDragAtEnd &&
                    lb->dragTarget == arrlen(lb->list->layers) - 1) {
                    MoveIdxBottomLayerList(lb->list, fromIndex);
                } else {
                    MoveIdxLayerList(lb->list, fromIndex, toIndex);
                }
            }

            lb->draggingLayer = false;
            lb->dragLayer = NULL;
        }

        // AutoScrolling when dragging
        if (lb->draggingLayer && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (mpos.y < lb->layersRect.y) {
                lb->scroll.y -= 200 * GetFrameTime();
            }
            if (mpos.y > lb->layersRect.y + lb->layersRect.height) {
                lb->scroll.y += 200 * GetFrameTime();
            }

            if (lb->dragTarget != lb->dragLayer->index) {
                GuiLabel(
                    (Rectangle){mpos.x, mpos.y, LAYER_NAME_WIDTH,
                                LAYER_ITEM_HEIGHT},
                    TextFormat(
                        "%s | #%d -> %c #%d", lb->dragLayer->name,
                        lb->dragLayer->index, lb->putDragAtEnd ? 'v' : '^',
                        lb->dragTarget
                    )
                );
            }
        }

        layerContentRect.height = py - layerContentRect.height;
        DrawScrollBars(
            lb, (Rectangle){0, 0, layersBounds.width,
                            (LAYER_ITEM_HEIGHT)*lb->list->count}
        );

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
            CheckCollisionPointRec(mpos, bounds) && !locked) {
            lb->menuOpen = !lb->menuOpen;
            if (lb->menuOpen) {
                lb->menuPos.x = mpos.x;
                lb->menuPos.y = mpos.y;
            }
        }

        if (lb->menuOpen) {
            if (!locked && IsKeyPressed(KEY_ESCAPE)) {
                lb->menuSelLayer = NULL;
                lb->menuOpen = false;
            }
            CtxMenuResult ctxRes = CtxMenu(lb);
            if (ctxRes == CMR_CLOSE) {
                lb->menuSelLayer = NULL;
                lb->menuOpen = false;
            }
        }

        if (lb->wLayerOpts.p.active) {
            if (!locked && IsKeyPressed(KEY_ESCAPE)) {
                lb->wLayerOpts.p.active = false;
            }
            WinStatus optStatus = WLayerOpts(&lb->wLayerOpts);
            if (optStatus == WIN_OK) {
                lb->curLayer->opacity = (lb->wLayerOpts.opacityVal / 100.0f);
                if (!TextIsEqual(lb->curLayer->name, lb->wLayerOpts.name)) {
                    free(lb->curLayer->name);
                    lb->curLayer->name = MakeString(lb->wLayerOpts.name);
                }
                lb->wLayerOpts.p.active = false;
            }
            if (optStatus == WIN_CANCEL || optStatus == WIN_CLOSE) {
                lb->wLayerOpts.p.active = false;
                lb->curLayer->opacity = lb->wLayerOpts.ogOpacity;
            }

            if (lb->wLayerOpts.opacityClicked) {
                lb->curLayer->opacity = (lb->wLayerOpts.opacityVal / 100.0f);
            }
        }
    }
    return -1;
}
