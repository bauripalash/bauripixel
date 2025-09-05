#include "../include/widgets/canvas.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../external/rlgl.h"
#include "../include/canvas/draw.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/drawtools.h"
#include "../include/options.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

CanvasState NewCanvas(int w, int h) {
    CanvasState c = {0};
    c.prop = NewWidgetProp();
    c.prop.bounds = (Rectangle){0};
    c.gridSize = (Vector2){w, h};
    c.hoverX = 0;
    c.hoverY = 0;
    c.anchor = (Vector2){0, 0};
    c.bottomAnchor = (Vector2){0, 0};
    c.curTool = DT_PENCIL;

    c.scroll = (Vector2){0, 0};
    c.zoomMin = 0.2f;
    c.zoomMax = INIT_CELL_SIZE;

    c.scrollThickness = 10.0f;

    c.current = ColorBlack;
    c.pxSize = INIT_CELL_SIZE;
    c.camera = (Camera2D){0};
    c.drawArea = (Rectangle){0};
    c.hScrollRect = (Rectangle){0};
    c.vScrollRect = (Rectangle){0};
    c.vScrollDragging = false;
    c.hScrollDragging = false;
    c.panning = false;
    c.enablePanning = false;

    c.camera.target = (Vector2){0, 0};

    c.camera.zoom = 1.0f;

    c.bgColor = BLANK;
    c.bgImg = GenImageChecked(
        c.gridSize.x, c.gridSize.y, 8, 8, ColorCheckerLight, ColorCheckerDark
    );
    c.bgTxt = LoadTextureFromImage(c.bgImg);

    c.previewImg = GenImageColor(c.gridSize.x, c.gridSize.y, BLANK);
    c.previewTxt = LoadTextureFromImage(c.previewImg);
    c.camera.target = Vector2Zero();
    c.camera.offset = Vector2Zero();
    c.point = Vector2Zero();

    // --
    c.brushSize = 1;
    c.brushShape = BSP_SQAURE;
    c.brushDragging = false;
    c.lastBrushPos = Vector2Negetive();

    c.eraserDragging = false;
    c.lastEraserPos = Vector2Negetive();

    c.lineStart = Vector2Zero();
    c.lineDragging = false;

    c.circleStart = (Vector2){0, 0};
    c.circleDragging = false;

    c.rectStart = (Vector2){0, 0};
    c.rectDragging = false;

    // --
    //
    c.drawArea4 = (Vector4){0};
    c.canvasArea4 = (Vector4){0};

    c.sbHThumbRect = (Rectangle){0};
    c.hoverHThumb = false;
    c.sbVThumbRect = (Rectangle){0};
    c.hoverVThumb = false;

    c.guideGridSize = (Vector2){8, 8};
    c.guideGridOffset = (Vector2){0, 0};
    c.guidGridThickness = 2.0f;
    c.guideGridColor = BpSolidColor(0x0, 0x0, 0xff);

    return c;
}

void FreeCanvas(CanvasState *c) {
    if (c == NULL) {
        return;
    }

    if (c->bgImg.data != NULL) {
        UnloadTexture(c->bgTxt);
        UnloadImage(c->bgImg);
    }

    if (c->previewImg.data != NULL) {

        UnloadTexture(c->previewTxt); // Do we need this?
        UnloadImage(c->previewImg);
    }
}

void UpdateCanvasLayers(
    CanvasState *state, LayerList *list, LayerObj *current
) {
    if (state == NULL || list == NULL || list->layers == NULL) {
        return;
    }

    state->layers = list;
    state->curLayer = current;
}

static void updateBounds(CanvasState *c) {
    c->prop.bounds.x = c->anchor.x + CANVAS_MARGIN_L;
    c->prop.bounds.y = c->anchor.y + CANVAS_MARGIN_TB;
    c->prop.bounds.width =
        c->bottomAnchor.x - c->anchor.x - (CANVAS_MARGIN_L + CANVAS_MARGIN_R);
    c->prop.bounds.height =
        c->bottomAnchor.y - c->anchor.y - CANVAS_MARGIN_TB * 2;

    Rectangle bounds = c->prop.bounds;
    c->drawArea = (Rectangle){
        bounds.x + CANVAS_DRAW_MARGIN + c->scrollThickness,
        bounds.y + CANVAS_DRAW_MARGIN + c->scrollThickness,
        bounds.width - (CANVAS_DRAW_MARGIN * 2) - c->scrollThickness * 3,
        bounds.height - (CANVAS_DRAW_MARGIN * 2) - c->scrollThickness * 3
    };

    c->vScrollRect =
        (Rectangle){c->drawArea.x + c->drawArea.width, c->drawArea.y,
                    c->scrollThickness, c->drawArea.height};

    c->hScrollRect =
        (Rectangle){c->drawArea.x, c->drawArea.y + c->drawArea.height,
                    c->drawArea.width, c->scrollThickness};
}

void UpdateCanvasAnchor(CanvasState *state, Vector2 anchor, Vector2 bottom) {

    float oldX = state->drawArea.x;
    if (anchor.x != -1) {
        state->anchor.x = anchor.x;
    }

    if (anchor.y != -1) {
        state->anchor.y = anchor.y;
    }

    if (bottom.x != -1) {
        state->bottomAnchor.x = bottom.x;
    }

    if (bottom.y != -1) {
        state->bottomAnchor.y = bottom.y;
    }

    updateBounds(state);
    state->point.x -= oldX - state->drawArea.x;
    state->camera.target.x = state->point.x;
}

void SetCanvasAnchor(CanvasState *state, Vector2 anchor, Vector2 bottom) {
    if (anchor.x != -1) {
        state->anchor.x = anchor.x;
    }

    if (anchor.y != -1) {
        state->anchor.y = anchor.y;
    }

    if (bottom.x != -1) {
        state->bottomAnchor.x = bottom.x;
    }

    if (bottom.y != -1) {
        state->bottomAnchor.y = bottom.y;
    }

    updateBounds(state);

    Vector2 txtCenter = {
        state->drawArea.x + state->gridSize.x * 0.5f,
        state->drawArea.y + state->gridSize.y * 0.5f,
    };

    Vector2 daCenter = {
        state->drawArea.x + state->drawArea.width * 0.5f,
        state->drawArea.y + state->drawArea.height * 0.5f
    };

    float zm = state->drawArea.width / state->gridSize.x / 2.0f;

    state->camera.zoom = zm;
    state->camera.target = txtCenter;
    state->point = txtCenter;
    state->camera.offset = daCenter;
    CenterAlignCanvas(state);
}

void CenterAlignCanvas(CanvasState *state) {
    Vector2 txtCenter = {
        state->drawArea.x + state->gridSize.x * 0.5f,
        state->drawArea.y + state->gridSize.y * 0.5f,
    };

    Vector2 daCenter = {
        state->drawArea.x + state->drawArea.width * 0.5f,
        state->drawArea.y + state->drawArea.height * 0.5f
    };

    float zm = state->drawArea.width / state->gridSize.x / 2.0f;
    state->camera.zoom = zm;
    state->camera.target = txtCenter;
    state->point = txtCenter;
    state->camera.offset = daCenter;
}

void SetCurrentCanvasColor(CanvasState *state, Color color) {
    state->current = color;
}

#define DA_ROUNDNESS     0.01f

#define TMR              3
#define TMW              50

#define SCROLL_ROUNDNESS 0.9

bool CanvasScrollBarsDraw(CanvasState *state) {
    Rectangle vScrollBounds = state->vScrollRect;
    Rectangle hScrollBounds = state->hScrollRect;
    Rectangle vThumbRect = state->sbVThumbRect;
    Rectangle hThumbRect = state->sbHThumbRect;

    Color scrollBgV = GetColor(OptThemeGet(T_SCROLLBAR_BG));
    Color scrollBgH = GetColor(OptThemeGet(T_SCROLLBAR_BG));
    Color scrollFgV = GetColor(OptThemeGet(T_SCROLLBAR_FG));
    Color scrollFgH = GetColor(OptThemeGet(T_SCROLLBAR_FG));

    if (state->vScrollDragging) {
        scrollBgV = GetColor(OptThemeGet(T_SCROLLBAR_CLK_BG));
        scrollFgV = GetColor(OptThemeGet(T_SCROLLBAR_CLK_FG));
    }

    if (state->hScrollDragging) {
        scrollBgH = GetColor(OptThemeGet(T_SCROLLBAR_CLK_BG));
        scrollFgH = GetColor(OptThemeGet(T_SCROLLBAR_CLK_FG));
    }

    if (state->hoverHThumb) {
        scrollFgH = GetColor(OptThemeGet(T_SCROLLBAR_HVR_FG));
    }

    if (state->hoverVThumb) {
        scrollFgV = GetColor(OptThemeGet(T_SCROLLBAR_HVR_FG));
    }

    DrawRectangleRounded(vScrollBounds, SCROLL_ROUNDNESS, 0, scrollBgV);
    DrawRectangleRounded(hScrollBounds, SCROLL_ROUNDNESS, 0, scrollBgH);
    DrawRectangleRounded(hThumbRect, SCROLL_ROUNDNESS, 0, scrollFgH);
    DrawRectangleRounded(vThumbRect, SCROLL_ROUNDNESS, 0, scrollFgV);

    return false;
}

bool CanvasScrollBarsLogic(CanvasState *state) {
    Vector4 drawArea = state->drawArea4;
    Vector4 canvas = state->canvasArea4;
    Rectangle vBounds = state->vScrollRect;
    Rectangle hBounds = state->hScrollRect;

    bool locked = GuiIsLocked();

    Vector2 mpos = GetMousePosition();

    Vector2 dTl = {drawArea.x, drawArea.y};
    Vector2 dBr = {drawArea.z, drawArea.w};
    Vector2 cTl = {canvas.x, canvas.y};
    Vector2 cBr = {canvas.z, canvas.w};

    float viewWidth = dBr.x - dTl.x;
    float viewHeight = dBr.y - dTl.y;

    float canvasWidth = cBr.x - cTl.x;
    float canvasHeight = cBr.y - cTl.y;

    // total area that can the canvas go around
    float scrollWidth = viewWidth + canvasWidth * 2;
    float scrollHeight = viewHeight + canvasHeight * 2;

    // 1 px of scrollbar is equal to ____ px in draw area
    float xScale = scrollWidth / hBounds.width;
    float yScale = scrollHeight / vBounds.height;

    /* Limits of travel for canvas
float minPosX = -canvasWidth;
float maxPosX = viewWidth;
float minPosY = -canvasHeight;
float maxPosY = viewHeight;
    */

    // distance from bottom edge of draw area to
    // top edge of the canvas
    float distEdgeY = dBr.y - cTl.y;

    // distance from right edge of draw area to
    // left edge of canvas
    float distEdgeX = dBr.x - cTl.x;

    float oldScrollX = distEdgeX / xScale;
    float oldScrollY = distEdgeY / yScale;

    state->scroll.x = oldScrollX;
    state->scroll.y = oldScrollY;

    float hThumbX = hBounds.x + state->scroll.x;
    float vThumbY = vBounds.y + state->scroll.y;
    float hThumbWidth = (canvasWidth / scrollWidth) * hBounds.width;
    float vThumbHeight = (canvasHeight / scrollHeight) * vBounds.height;
    Rectangle hThumbRect = {hThumbX, hBounds.y, hThumbWidth, hBounds.height};

    Rectangle vThumbRect = {vBounds.x, vThumbY, vBounds.width, vThumbHeight};

    if (CheckCollisionPointRec(mpos, hBounds) && !locked) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            state->hScrollDragging = true;
        }

        float wheel = GetMouseWheelMove();

        if (wheel != 0.0f) {
            float scale = 10.0f * wheel;
            state->scroll.x += scale;
        }
    }

    if (CheckCollisionPointRec(mpos, vBounds) && !locked) {

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            state->vScrollDragging = true;
        }

        float wheel = GetMouseWheelMove();

        if (wheel != 0.0f) {
            float scale = 10.0f * wheel;
            state->scroll.y += scale;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !locked) {
        state->vScrollDragging = false;
        state->hScrollDragging = false;
    }

    if (state->vScrollDragging) {
        Vector2 delta = GetMouseDelta();
        state->scroll.y += delta.y;
    }

    if (state->hScrollDragging) {
        Vector2 delta = GetMouseDelta();
        state->scroll.x += delta.x;
    }
    state->scroll.x = Clamp(state->scroll.x, 0, hBounds.width - hThumbWidth);
    state->scroll.y = Clamp(state->scroll.y, 0, vBounds.height - vThumbHeight);

    state->point.x += (state->scroll.x - oldScrollX) * xScale;
    state->point.y += (state->scroll.y - oldScrollY) * yScale;

    state->sbVThumbRect = vThumbRect;
    state->sbHThumbRect = hThumbRect;
    state->hoverHThumb = CheckCollisionPointRec(mpos, hThumbRect) && !locked;
    state->hoverVThumb = CheckCollisionPointRec(mpos, vThumbRect) && !locked;

    return state->vScrollDragging || state->hScrollDragging;
}

void handleTools(CanvasState *state) {
    bool atDrawArea =
        CheckCollisionPointRec(GetMousePosition(), state->drawArea);
    bool atCanvas = CheckCollisionPointRec(
        GetScreenToWorld2D(GetMousePosition(), state->camera),
        (Rectangle){state->drawArea.x, state->drawArea.y, state->gridSize.x,
                    state->gridSize.y}
    );

    if (state->curTool != DT_PAN) {
        state->enablePanning = false;
    }
}

bool CanvasLogic(CanvasState *state) {
    if (state->prop.active) {
        updateBounds(state);
        handleTools(state);
        bool locked = GuiIsLocked();
        Vector2 mpos = GetMousePosition();

        Rectangle drawArea = state->drawArea;
        Rectangle canvasArea = {
            drawArea.x, drawArea.y, state->gridSize.x, state->gridSize.y
        };

        bool isHovering = CheckCollisionPointRec(mpos, drawArea) && !locked;
        state->hoverCanvas = isHovering;

        if (isHovering) {
            float wheel = GetMouseWheelMove();
            if (wheel != 0 && !IsKeyDown(KEY_LEFT_CONTROL)) {
                // Mouse position in drawing area
                Vector2 wMpos = GetScreenToWorld2D(mpos, state->camera);
                state->camera.offset = mpos;
                state->camera.target = wMpos;
                state->point = wMpos;

                float scale = 0.2f * wheel;
                state->camera.zoom = Clamp(
                    expf(logf(state->camera.zoom) + scale), 0.125f, 64.0f
                );

            } // wheel != 0

            if (state->enablePanning && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 delta =
                    Vector2Scale(GetMouseDelta(), -1.0 / state->camera.zoom);
                state->point = Vector2Add(state->point, delta);
                state->camera.target = state->point;
                state->panning = true;

            } else {
                state->panning = false;
            } // enablePanning

        } // isHovering

        Vector2 dTopLeft = GetScreenToWorld2D(
            (Vector2){drawArea.x, drawArea.y}, state->camera
        );

        Vector2 dBottomRight = GetScreenToWorld2D(
            (Vector2){drawArea.x + drawArea.width,
                      drawArea.y + drawArea.height},
            state->camera
        );

        Vector2 cTopLeft = {canvasArea.x, canvasArea.y};

        Vector2 cBottomRight = {
            canvasArea.x + canvasArea.width, canvasArea.y + canvasArea.height
        };

        /* How much is left in each side
        float emptyLeft = cTopLeft.x - dTopLeft.x;
        float emptyRight = dBottomRight.x - cBottomRight.x;
        float emptyTop = cTopLeft.y - dTopLeft.y;
        float emptyBottom = dBottomRight.y - cBottomRight.y;
        */

        state->drawArea4.x = dTopLeft.x;
        state->drawArea4.y = dTopLeft.y;
        state->drawArea4.z = dBottomRight.x;
        state->drawArea4.w = dBottomRight.y;

        state->canvasArea4.x = cTopLeft.x;
        state->canvasArea4.y = cTopLeft.y;
        state->canvasArea4.z = cBottomRight.x;
        state->canvasArea4.w = cBottomRight.y;
        CanvasScrollBarsLogic(state);

        if (state->panning && !locked) {
            float outsideLeft = (canvasArea.x + canvasArea.width) - dTopLeft.x;
            float outsideRight = canvasArea.x - dBottomRight.x;
            float outsideTop = (canvasArea.y + canvasArea.height) - dTopLeft.y;
            float outsideBottom = canvasArea.y - dBottomRight.y;

            if (outsideLeft < 0) {
                state->point.x += outsideLeft;
            }

            if (outsideRight > 0) {
                state->point.x += outsideRight;
            }

            if (outsideTop < 0) {
                state->point.y += outsideTop;
            }

            if (outsideBottom > 0) {
                state->point.y += outsideBottom;
            }

            state->vScrollDragging = false;
            state->hScrollDragging = false;
        }

        DrawingCanvasLogic(state, canvasArea);

        state->camera.target.x = state->point.x;
        state->camera.target.y = state->point.y;
    }
    return false;
}

static const float cellSize = 8.0f;

void DrawGuideGrid(CanvasState *state) {
    Color clr = state->guideGridColor;
    int xSize = (int)state->guideGridSize.x;
    int ySize = (int)state->guideGridSize.y;
    int cHeight = (int)state->gridSize.y;
    int cWidth = (int)state->gridSize.x;

    rlPushMatrix();
    rlTranslatef(state->drawArea.x, state->drawArea.y, 0.0f);
    rlBegin(RL_LINES);
    rlSetLineWidth(state->guidGridThickness);

    rlColor4ub(clr.r, clr.g, clr.b, clr.a);

    float offsetX = state->guideGridOffset.x;
    float offsetY = state->guideGridOffset.y;
    for (int x = offsetX; x <= cWidth; x += xSize) {
        rlVertex2f(x, offsetY);
        rlVertex2f(x, cHeight);
    }

    for (int y = offsetY; y <= cHeight; y += ySize) {
        rlVertex2f(offsetX, y);
        rlVertex2f(cWidth, y);
    }

    rlEnd();
    rlPopMatrix();
}

bool CanvasDraw(CanvasState *state) {
    if (state->prop.active) {
        Rectangle drawArea = {
            state->drawArea.x, state->drawArea.y, state->drawArea.width,
            state->drawArea.height
        };
        Rectangle bounds = state->prop.bounds;
        BpPanelBorder(bounds, 3);
        CanvasScrollBarsDraw(state);

        BeginScissorMode(
            drawArea.x, drawArea.y, drawArea.width, drawArea.height
        );
        GuiGrid(drawArea, NULL, state->gridSize.x * 2.0f, 1, NULL);
        Rectangle drawRect = {
            drawArea.x, drawArea.y, state->gridSize.x, state->gridSize.y
        };
        BeginMode2D(state->camera);
        {
            DrawingCanvasDraw(state, drawRect);
            DrawGuideGrid(state);
        }
        EndMode2D();

        EndScissorMode();
    }
    return false;
}
