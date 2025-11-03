#include "../include/widgets/layerbar.h"
#include "../external/raymath.h"
#include "../external/stb/stb_ds.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/options.h"
#include "../include/utils.h"
#include <stdbool.h>
#include <stdlib.h>

// Layerbar widget initial height
#define LB_INIT_HEIGHT       120
// Layerbar widget minimum height
#define LB_MIN_HEIGHT        50
// Layer Left, right margin 
#define LB_MARGIN_LR         10
// Layerbar Top, bottom margin
#define LB_MARGIN_TB         10
// Layerbar Left, right padding
#define LB_PADDING_LR        10
// Layerbar Top, bottom padding
#define LB_PADDING_TB        10

// Layerbar resize handle thickness
#define HANDLE_THICKNESS     20
// Height of each layer item
#define LAYER_ITEM_HEIGHT    35
// Default name label width 
#define DEF_LAYER_NAME_WIDTH 200
// Margin for each layer item
#define LAYER_ITEM_MARGIN    5

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
    lb.framesRect = (Rectangle){0};
    lb.wLayerOpts = NewWLayerOpts();

    lb.layerNameWidth = DEF_LAYER_NAME_WIDTH;

    lb.menuSelLayer = NULL;
    lb.menuOpen = false;
    lb.menuPos = Vector2Zero();

    lb.draggingLayer = false;
    lb.dragLayer = NULL;

    lb.curFrame = 0;
    lb.timelineState = TIMELINE_STOP;

    return lb;
}

void FreeLayerBar(LayerBarState *lb) { return; }

#define SCROLLT              10
#define LAYER_NAME_FRAME_GAP 8
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

    lb->framesRect = (Rectangle){
        lb->layersRect.x + lb->layerNameWidth + LAYER_NAME_FRAME_GAP,
        lb->layersRect.y,
        lb->layersRect.width - (lb->layerNameWidth + LAYER_NAME_FRAME_GAP),
        lb->layersRect.height
    };

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

        FrameObj *frame = NewFrameObj(lr->width, lr->height);
        if (frame == NULL) {
            return false;
        }

        AddToFrameList(lr->flist, frame);
    }
    return true;
}

static bool createDupFrame(LayerBarState *lb) {
    int fcount = lb->curLayer->flist->count;
    for (int i = 0; i < lb->list->count; i++) {
        LayerObj *lr = lb->list->layers[i];
        if (lr == NULL) {
            return false;
        }

        FrameObj *frame;
        if (fcount > 0) {
            frame = DuplicateFrameObj(lr->flist->frames[lb->curFrame]);
        } else {
            frame = NewFrameObj(lr->width, lr->height);
        }
        if (frame == NULL) {
            return false;
        }

        AddToFrameList(lr->flist, frame);
    }
    return true;
}

bool LayerItemFrameDraw(
    LayerBarState *lb, Rectangle bounds, LayerObj *layer, bool isCur
) {

    int framecount = layer->flist->count;
    Rectangle cellRect = {
        bounds.x, bounds.y, LAYER_ITEM_HEIGHT, LAYER_ITEM_HEIGHT
    };

    for (int i = 0; i < framecount; i++) {
        FrameObj *f = layer->flist->frames[i];
        bool clicked = false;
        if (lb->curFrame == i && isCur) {
            clicked = BpFramePrevActive(cellRect, f, false);
        } else {
            clicked = BpFramePrevBox(cellRect, f, false);
        }

        if (clicked &&
            CheckCollisionPointRec(GetMousePosition(), lb->framesRect)) {
            lb->curFrame = i;
            lb->selectedLayer = layer;
        }

        cellRect.x += cellRect.width;
    }

    return CheckCollisionPointRec(GetMousePosition(), bounds) &&
           IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !GuiIsLocked();
}

bool LayerItemDraw(
    LayerBarState *lb, Rectangle bounds, LayerObj *layer, bool isCur
) {
    int ogBorder = OptThemeGetSet(T_PANEL_BORDER, OptThemeGet(T_LAYER_BRDR));
    bool locked = GuiIsLocked();

    Vector2 mpos = GetMousePosition();

    bool hover = CheckCollisionPointRec(mpos, bounds) && !locked &&
                 CheckCollisionPointRec(mpos, lb->layersRect);
    bool clicked = hover && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
    float halfHeight = bounds.height / 2.0f;

    Rectangle toolRect = {
        bounds.x + 5,
        bounds.y,
        50,
        bounds.height,
    };
    Rectangle nameRect = {
        toolRect.x + toolRect.width, toolRect.y, bounds.width - 50,
        bounds.height
    };

    BpPanelBorder(bounds, 2);

    Rectangle btnRect = {toolRect.x, toolRect.y, 20, toolRect.height};

    GuiIconName visIcon = layer->visible ? ICON_EYE_ON : ICON_EYE_OFF;
    if (BpLabelButton(btnRect, GuiIconText(visIcon, NULL))) {
        layer->visible = !layer->visible;
    }

    btnRect.x += btnRect.width;

    GuiIconName lockIcon = ICON_LOCK_OPEN;
    if (BpLabelButton(btnRect, GuiIconText(lockIcon, NULL))) {
    }

    GuiLabel(nameRect, TextFormat("#%d %s", layer->index, layer->name));

    if (hover && (lb->draggingLayer)) {
        lb->dragTarget = layer->index;
        Rectangle upperBounds = {bounds.x, bounds.y, bounds.width, halfHeight};
        if (CheckCollisionPointRec(mpos, upperBounds)) {
            lb->putDragAtEnd = false;
        } else {
            lb->putDragAtEnd = true;
        }
    }

    if (hover && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
        CheckCollisionPointRec(mpos, lb->layersRect)) {
        lb->menuSelLayer = layer;
    }

    if (CheckCollisionPointRec(mpos, nameRect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !locked &&
        CheckCollisionPointRec(mpos, lb->layersRect)) {
        lb->draggingLayer = true;
        lb->dragLayer = layer;
    }

    OptThemeSet(T_PANEL_BORDER, ogBorder);

    return clicked;
}
static void frwdFrame(LayerBarState *lb) {
    if (lb->curFrame < (lb->curLayer->flist->count - 1)) {
        lb->curFrame++;
    }
}

static void bkwdFrame(LayerBarState *lb) {
    if (lb->curFrame > 0) {
        lb->curFrame--;
    }
}

static void upLayer(LayerBarState *lb) {

    if (lb->curLayer->index > 0) {
        int targetIndex = lb->curLayer->index - 1;
        LayerObj *obj = lb->list->layers[targetIndex];
        lb->selectedLayer = obj;
    }
}

static void dwnLayer(LayerBarState *lb) {
    if (lb->curLayer->index < (lb->list->count - 1)) {
        LayerObj *obj = lb->list->layers[lb->curLayer->index + 1];
        lb->selectedLayer = obj;
    }
}

int LayerBarLogic(LayerBarState *lb, double dt) {
    if (lb->p.active) {
        updateBounds(lb);
        bool locked = GuiIsLocked();

        Rectangle bounds = lb->p.bounds;
        Rectangle handleRect = {
            bounds.x, bounds.y - (HANDLE_THICKNESS / 2.0f), bounds.width,
            HANDLE_THICKNESS
        };

        Vector2 mpos = GetMousePosition();

        if (CheckCollisionPointRec(mpos, bounds)) {
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
                bkwdFrame(lb);
            }

            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
                frwdFrame(lb);
            }

            if (IsKeyPressed(KEY_UP)) {
                upLayer(lb);
            }

            if (IsKeyPressed(KEY_DOWN)) {
                dwnLayer(lb);
            }
        }

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
        // TODO: Delete and select prev/next layer
        if (BpLabelButton(labelRect, "Delete Layer")) {
            if (/*lb->list->count >= 2*/ false) { // DISABLE for now
                LayerObj *lr =
                    RemoveIdxLayerList(lb->list, lb->menuSelLayer->index);
                FreeLayerObj(lr);
            }
            result = CMR_CLOSE;
        }
    }
    return result;
}

int LayerBarDraw(LayerBarState *lb, double dt) {
    if (lb->p.active) {
        lb->anypopup = lb->wLayerOpts.p.active;
        bool locked = GuiIsLocked();
        Vector2 mpos = GetMousePosition();

        Rectangle bounds = lb->p.bounds;
        // Bounds for tool buttons
        Rectangle toolBarBounds = lb->toolsRect;
        // Bounds for layers (with frames)
        Rectangle layersBounds = lb->layersRect;
        // Bounds for frames only (part of layersBounds)
        Rectangle framesBounds = lb->framesRect;

        // BpRoundedPanel(bounds, 2, 0.125, true);
        BpPanelBorder(bounds, 2);
        Rectangle toolBtnRect = toolBarBounds;
        toolBtnRect.width = toolBtnRect.height;

        if (BpTextButton(toolBtnRect, GuiIconText(ICON_TARGET_SMALL, NULL))) {
            createNewLayer(lb, lb->curLayer);
        }
        toolBtnRect.x += toolBtnRect.width;
        if (BpTextButton(toolBtnRect, GuiIconText(ICON_PLAYER_JUMP, NULL))) {
            createDupFrame(lb);
            // lb->curFrame++;
        }

        toolBtnRect.x += toolBtnRect.width + LAYER_ITEM_MARGIN;
        if (BpTextButton(
                toolBtnRect, GuiIconText(ICON_PLAYER_PREVIOUS, NULL)
            )) {
            if (lb->curFrame != 0) {
                lb->curFrame--;
            }
        }
        toolBtnRect.x += toolBtnRect.width;
        GuiIconName playIcon = lb->timelineState == TIMELINE_PLAY
                                   ? ICON_PLAYER_PAUSE
                                   : ICON_PLAYER_PLAY;
        if (BpTextButton(toolBtnRect, GuiIconText(playIcon, NULL))) {
            if (lb->timelineState == TIMELINE_PLAY) {
                lb->timelineState = TIMELINE_PAUSE;
            } else {
                lb->timelineState = TIMELINE_PLAY;
            }
        }
        toolBtnRect.x += toolBtnRect.width;
        if (BpTextButton(toolBtnRect, GuiIconText(ICON_PLAYER_STOP, NULL))) {
            lb->timelineState = TIMELINE_STOP;
            lb->curFrame = 0;
        }
        toolBtnRect.x += toolBtnRect.width;
        if (BpTextButton(toolBtnRect, GuiIconText(ICON_PLAYER_NEXT, NULL))) {
            if (lb->curFrame != (lb->curLayer->flist->count - 1)) {
                lb->curFrame++;
            }
        }

        Rectangle layerContentRect = layersBounds;

        BpPanelBorder(
            (Rectangle){layersBounds.x - 1, layersBounds.y - 1,
                        layersBounds.width + 4, layersBounds.height + 4},
            2
        );

        int layerCount = lb->list->count;
        float posY = layersBounds.y + LAYER_ITEM_MARGIN;
        float pyinc = LAYER_ITEM_HEIGHT + 1;
        Rectangle activeRect = {0};
        Rectangle layerNamesRect = {
            layersBounds.x, layersBounds.y, lb->layerNameWidth + 8,
            layersBounds.height
        };
        BeginScissorModeRec(layerNamesRect);

        for (int i = 0; i < layerCount; i++) {
            LayerObj *layer = lb->list->layers[i];
            Rectangle layerNameBtn = {
                layersBounds.x + LAYER_ITEM_MARGIN,
                posY - lb->scroll.y,
                lb->layerNameWidth,
                LAYER_ITEM_HEIGHT,
            };
            bool isCur = (i == lb->curLayer->index);
            if (LayerItemDraw(lb, layerNameBtn, layer, isCur) &&
                CheckCollisionPointRec(mpos, layerNamesRect)) {
                lb->selectedLayer = layer;
            }
            if (isCur) {
                activeRect =
                    (Rectangle){layerNameBtn.x, layerNameBtn.y,
                                layerNameBtn.width, layerNameBtn.height};
            }

            if (lb->dragTarget == layer->index && lb->draggingLayer &&
                lb->dragTarget != lb->curLayer->index) {
                float dragY = layerNameBtn.y;
                if (lb->putDragAtEnd) {
                    dragY += LAYER_ITEM_HEIGHT - 8;
                }

                DrawRectangleRec(
                    (Rectangle){layerNameBtn.x, dragY,
                                lb->layersRect.width - LAYER_ITEM_MARGIN, 8},
                    BpColorSetAlpha(ColorVGreen, 200)
                );
            }

            posY += pyinc;
        }

        int ogBorder =
            OptThemeGetSet(T_PANEL_BORDER, OptThemeGet(T_LAYER_ACTIVE_BRDR));

        BpPanelOnlyBorder(activeRect, 2);
        OptThemeSet(T_PANEL_BORDER, ogBorder);
        EndScissorMode();
        BeginScissorModeRec(framesBounds);

        ogBorder = OptThemeGetSet(T_PANEL_BORDER, OptThemeGet(T_LAYER_BRDR));

        int frameCount = lb->curLayer->flist->count;
        float fposY = layersBounds.y + LAYER_ITEM_MARGIN;

        for (int i = 0; i < layerCount; i++) {
            LayerObj *layer = lb->list->layers[i];
            Rectangle layerFrameRect = {
                (framesBounds.x + 1) - lb->scroll.x, fposY - lb->scroll.y,
                (LAYER_ITEM_HEIGHT * frameCount), LAYER_ITEM_HEIGHT
            };

            bool isCur = (lb->curLayer->index == i);
            if (LayerItemFrameDraw(lb, layerFrameRect, layer, isCur) &&
                CheckCollisionPointRec(mpos, lb->framesRect)) {
                // lb->selectedLayer = layer;
            }

            fposY += pyinc;
        }

        Rectangle activeFramesRect = {
            (framesBounds.x + 1) + lb->curFrame * LAYER_ITEM_HEIGHT -
                lb->scroll.x,
            layersBounds.y + LAYER_ITEM_MARGIN - lb->scroll.y,
            LAYER_ITEM_HEIGHT,
            (layerCount * (LAYER_ITEM_HEIGHT + 1)),
        };

        BpPanelOnlyClrBorder(activeFramesRect, 2, ColorVWhite);

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
                    (Rectangle){mpos.x, mpos.y, lb->layerNameWidth,
                                LAYER_ITEM_HEIGHT},
                    TextFormat(
                        "%s | #%d -> %c #%d", lb->dragLayer->name,
                        lb->dragLayer->index, lb->putDragAtEnd ? 'v' : '^',
                        lb->dragTarget
                    )
                );
            }
        }

        layerContentRect.height = posY - layerContentRect.height;
        Rectangle usedRect = {
            framesBounds.x, framesBounds.y + LAYER_ITEM_MARGIN,
            (LAYER_ITEM_HEIGHT + 1) * frameCount,
            ((LAYER_ITEM_HEIGHT + 1) * layerCount) + LAYER_ITEM_MARGIN * 2
        };
        Rectangle scrollRect = lb->framesRect;
        scrollRect.width += 5;
        scrollRect.height += 5;
        
        BpScrollPanel(
            scrollRect, usedRect, &lb->scroll, lb->framesRect,
            &lb->hScrollDragging, &lb->vScrollDragging, &lb->layersRect
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
            WinStatus optStatus = WLayerOpts(&lb->wLayerOpts, dt);
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
