#include "../include/widgets/canvas.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include "../include/drawtools.h"
#include "../include/options.h"
#include "../include/utils.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static Rectangle trect = {0};

CanvasState NewCanvas() {
    CanvasState c = {0};
    c.prop = NewWidgetProp();
    c.hoverX = 0;
    c.hoverY = 0;
    c.gridSize = (Vector2){DEFAULT_GRID_SIZE, DEFAULT_GRID_SIZE};
    c.anchor = (Vector2){0, 0};
    c.bottomAnchor = (Vector2){0, 0};
    c.curTool = DT_PENCIL;

    c.scroll = (Vector2){0, 0};
    c.zoomMin = 0.2f;
    c.zoomMax = INIT_CELL_SIZE;

    for (int i = 0; i < (int)c.gridSize.y; i++) {
        for (int j = 0; j < (int)c.gridSize.x; j++) {
            c.colors[i][j] = ColorWhite;
        }
    }
    c.scrollThickness = 10.0f;

    c.current = ColorBlack;
    c.pxSize = INIT_CELL_SIZE;
    c.prop.bounds.x = CANVAS_MARGIN_L;
    c.prop.bounds.y = CANVAS_MARGIN_TB;
    c.prop.bounds.width = GetScreenWidth() -
                          (CANVAS_MARGIN_L + CANVAS_MARGIN_R) -
                          c.bottomAnchor.x;
    c.prop.bounds.height = GetScreenHeight() - (CANVAS_MARGIN_TB * 2);

    c.camera = (Camera2D){0};

    Rectangle bounds = c.prop.bounds;
    c.drawArea = (Rectangle){
        bounds.x + CANVAS_DRAW_MARGIN + c.scrollThickness,
        bounds.y + CANVAS_DRAW_MARGIN + c.scrollThickness,
        bounds.width - (CANVAS_DRAW_MARGIN * 2) - c.scrollThickness * 3,
        bounds.height - (CANVAS_DRAW_MARGIN * 2) - c.scrollThickness * 3
    };

    c.hScrollRect = (Rectangle){c.drawArea.x, c.drawArea.y + c.drawArea.height,
                                c.drawArea.width, c.scrollThickness};

    c.vScrollRect = (Rectangle){c.drawArea.x + c.drawArea.width, c.drawArea.y,
                                c.scrollThickness, c.drawArea.height};
    c.vScrollDragging = false;
    c.hScrollDragging = false;
    c.panning = false;
    c.enablePanning = false;

    float zm = c.drawArea.width / c.gridSize.x / 2.0f;

    c.camera.target = (Vector2){0, 0};

    c.camera.zoom = zm;

    c.canvasImg = GenImageColor(c.gridSize.x, c.gridSize.y, WHITE);
    c.canvasTxt = LoadTextureFromImage(c.canvasImg);
    c.previewImg = GenImageColor(c.gridSize.x, c.gridSize.y, BLANK);
    c.previewTxt = LoadTextureFromImage(c.previewImg);

    Vector2 txtCenter = {
        c.drawArea.x + c.canvasTxt.width * 0.5f,
        c.drawArea.y + c.canvasTxt.height * 0.5f,
    };

    Vector2 daCenter = {
        c.drawArea.x + c.drawArea.width * 0.5f,
        c.drawArea.y + c.drawArea.height * 0.5f
    };

    c.camera.target = txtCenter;
    c.camera.offset = daCenter;
    c.point = txtCenter;

    // --
    c.brushSize = 1;
    c.lineStart = (Vector2){-1, -1};
    c.lineDragging = false;
    // --

    return c;
}

void updateBounds(CanvasState *c) {
    c->prop.bounds.x = c->anchor.x + CANVAS_MARGIN_L;
    c->prop.bounds.y = c->anchor.y + CANVAS_MARGIN_TB;
    c->prop.bounds.width = GetScreenWidth() -
                           (CANVAS_MARGIN_L + CANVAS_MARGIN_R) - c->anchor.x -
                           c->bottomAnchor.x;
    c->prop.bounds.height = GetScreenHeight() - (CANVAS_MARGIN_TB * 2) -
                            c->anchor.y - c->bottomAnchor.y;

    Rectangle bounds = c->prop.bounds;
    c->drawArea = (Rectangle){
        bounds.x + CANVAS_DRAW_MARGIN + c->scrollThickness,
        bounds.y + CANVAS_DRAW_MARGIN + c->scrollThickness,
        bounds.width - (CANVAS_DRAW_MARGIN * 2) - c->scrollThickness * 3,
        bounds.height - (CANVAS_DRAW_MARGIN * 2) - c->scrollThickness * 3
    };

    c->vScrollRect = (Rectangle){c->prop.bounds.x + c->prop.bounds.width -
                                     c->scrollThickness,
                                 c->prop.bounds.y, c->scrollThickness,
                                 c->prop.bounds.height};

    c->hScrollRect = (Rectangle){c->prop.bounds.x,
                                 c->prop.bounds.y + c->prop.bounds.height -
                                     c->scrollThickness,
                                 c->prop.bounds.width, c->scrollThickness};
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
    UpdateCanvasAnchor(state, anchor, bottom);

    Vector2 txtCenter = {
        state->drawArea.x + state->canvasTxt.width * 0.5f,
        state->drawArea.y + state->canvasTxt.height * 0.5f,
    };

    Vector2 daCenter = {
        state->drawArea.x + state->drawArea.width * 0.5f,
        state->drawArea.y + state->drawArea.height * 0.5f
    };

    state->camera.target = txtCenter;
    state->point = txtCenter;
    state->camera.offset = daCenter;
}

void CenterAlignCanvas(CanvasState *state) {}

void SetCurrentCanvasColor(CanvasState *state, Color color) {
    state->current = color;
}

#define DA_ROUNDNESS     0.01f

#define TMR              3
#define TMW              50

#define SCROLL_ROUNDNESS 0.9

bool CanvasScrollBars(CanvasState *state, Vector4 drawArea, Vector4 canvas) {
    Rectangle vBounds = state->vScrollRect;
    Rectangle hBounds = state->hScrollRect;

    Color scrollBgV = GetColor(OptThemeGet(T_SCROLLBAR_BG));
    Color scrollBgH = GetColor(OptThemeGet(T_SCROLLBAR_BG));
    Color scrollFgV = GetColor(OptThemeGet(T_SCROLLBAR_FG));
    Color scrollFgH = GetColor(OptThemeGet(T_SCROLLBAR_FG));

    Camera2D cam = state->camera;
    float zoom = state->camera.zoom;

    Vector2 dTl = {drawArea.x, drawArea.y};
    Vector2 dBr = {drawArea.z, drawArea.w};
    Vector2 cTl = {canvas.x, canvas.y};
    Vector2 cBr = {canvas.z, canvas.w};

    float viewWidth = dBr.x - dTl.x;
    float viewHeight = dBr.y - dTl.y;

    float canvasWidth = cBr.x - cTl.x;
    float canvasHeight = cBr.y - cTl.y;

    float scrollWidth = viewWidth + canvasWidth * 2;
    float scrollHeight = viewHeight + canvasHeight * 2;

    // 1 px of scrollbar is equal to ____ px in draw area
    float xScale = scrollWidth / hBounds.width;
    float yScale = scrollHeight / vBounds.height;

    float minPosX = -canvasWidth;
    float maxPosX = viewWidth;
    float minPosY = -canvasHeight;
    float maxPosY = viewHeight;

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

    if (CheckCollisionPointRec(GetMousePosition(), hBounds)) {
        scrollBgH = GetColor(OptThemeGet(T_SCROLLBAR_HVR_BG));
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            state->hScrollDragging = true;
        }

        float wheel = GetMouseWheelMove();

        if (wheel != 0.0f) {
            float scale = 10.0f * wheel;
            state->scroll.x += scale;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), vBounds)) {

        scrollBgV = GetColor(OptThemeGet(T_SCROLLBAR_HVR_BG));
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            state->vScrollDragging = true;
        }

        float wheel = GetMouseWheelMove();

        if (wheel != 0.0f) {
            float scale = 10.0f * wheel;
            state->scroll.y += scale;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        state->vScrollDragging = false;
        state->hScrollDragging = false;
    }

    if (state->vScrollDragging) {
        scrollBgV = GetColor(OptThemeGet(T_SCROLLBAR_CLK_BG));
        scrollFgV = GetColor(OptThemeGet(T_SCROLLBAR_CLK_FG));
        Vector2 delta = GetMouseDelta();
        state->scroll.y += delta.y;
    }

    if (state->hScrollDragging) {
        scrollBgH = GetColor(OptThemeGet(T_SCROLLBAR_CLK_BG));
        scrollFgH = GetColor(OptThemeGet(T_SCROLLBAR_CLK_FG));
        Vector2 delta = GetMouseDelta();
        state->scroll.x += delta.x;
    }
    Vector2 mpos = GetMousePosition();
    if (CheckCollisionPointRec(mpos, hThumbRect)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            scrollFgH = GetColor(OptThemeGet(T_SCROLLBAR_CLK_FG));
        } else {
            scrollFgH = GetColor(OptThemeGet(T_SCROLLBAR_HVR_FG));
        }
    }

    if (CheckCollisionPointRec(mpos, vThumbRect)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            scrollFgV = GetColor(OptThemeGet(T_SCROLLBAR_CLK_FG));
        } else {
            scrollFgV = GetColor(OptThemeGet(T_SCROLLBAR_HVR_FG));
        }
    }

    state->scroll.x = Clamp(state->scroll.x, 0, hBounds.width - hThumbWidth);
    state->scroll.y = Clamp(state->scroll.y, 0, vBounds.height - vThumbHeight);

    state->point.x += (state->scroll.x - oldScrollX) * xScale;
    state->point.y += (state->scroll.y - oldScrollY) * yScale;

    DrawRectangleRounded(vBounds, SCROLL_ROUNDNESS, 0, scrollBgV);
    DrawRectangleRounded(hBounds, SCROLL_ROUNDNESS, 0, scrollBgH);
    DrawRectangleRounded(hThumbRect, SCROLL_ROUNDNESS, 0, scrollFgH);
    DrawRectangleRounded(vThumbRect, SCROLL_ROUNDNESS, 0, scrollFgV);

    return state->vScrollDragging || state->hScrollDragging;
}

bool cellHover(Rectangle rect, Camera2D cam) {
    return CheckCollisionPointRec(
        GetScreenToWorld2D(GetMousePosition(), cam), rect
    );
}

bool cellClicked(Rectangle rect, Camera2D cam) {
    return IsMouseButtonDown(MOUSE_BUTTON_LEFT) && cellHover(rect, cam);
}

// Returns position of mouse if inside a canvas.
// x,y => canvas cord || w,z => worldMousePos
Vector4 getCanvasPos(CanvasState *state, Rectangle canvasRect) {
    Vector2 mpos = GetMousePosition();
    Vector2 wMpos = GetScreenToWorld2D(mpos, state->camera);

    if (CheckCollisionPointRec(mpos, state->drawArea)) {
        if (CheckCollisionPointRec(wMpos, canvasRect)) {
            float px = floorf(wMpos.x - canvasRect.x);
            float py = floorf(wMpos.y - canvasRect.y);

            return (Vector4){px, py, wMpos.x, wMpos.y};
        }
    }
    return (Vector4){-1, -1, wMpos.x, wMpos.y};
}

bool validCanvasPos(Vector2 gridSize, Vector4 pos) {
    if ((pos.x < 0 || pos.x >= gridSize.x) ||
        (pos.y < 0 || pos.y >= gridSize.y)) {
        return false;
    }
    return true;
}

bool MouseIsAtCanvas(CanvasState *state, Rectangle rect) {
    Vector2 mpos = GetMousePosition();
    return (
        CheckCollisionPointRec(mpos, state->drawArea) &&
        CheckCollisionPointRec(GetScreenToWorld2D(mpos, state->camera), rect)
    );
}

bool PointIsAtCanvas(CanvasState *state, Rectangle rect, Vector2 point) {
    return false;
}

// Bresenham's line algorithm (modified) (prototype)
static void
BPDrawLine(CanvasState *state, Image *img, Vector2 a, Vector2 b, Color clr) {
    int x0 = (int)a.x;
    int y0 = (int)a.y;

    int x1 = (int)b.x;
    int y1 = (int)b.y;

    int bs = state->brushSize;

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        // ImageDrawPixel(img, x0, y0, clr);
        ImageDrawRectangle(img, x0, y0, bs, bs, clr);
        int e2 = 2 * err;

        if (e2 >= dy) {
            if (x0 == x1) {
                break;
            }

            err += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            if (y0 == y1)
                break;
            err += dx;
            y0 += sy;
        }
    }
}

// BUG: atCanvas should be `true` until the bottom-right corner of the brush is
// out of canvas (when brushSize > 1)
// for Left side -> BOTTOM-RIGHT is inside
// for Right Side -> TOP-LEFT is inside

void DrawingCanvas(CanvasState *state, Rectangle bounds) {
    Rectangle canvasRect = (Rectangle){state->drawArea.x, state->drawArea.y,
                                       state->gridSize.x, state->gridSize.y};
    Vector4 canvasPos = getCanvasPos(state, canvasRect);

    bool pxAtCanvas = validCanvasPos(state->gridSize, canvasPos);

    // int px = (int)canvasPos.x;
    // int py = (int)canvasPos.y;

    int pl = (int)ceilf(canvasPos.x - state->brushSize * 0.5f);
    int pt = (int)ceilf(canvasPos.y - state->brushSize * 0.5f);
    int pr = pl + state->brushSize;
    int pb = pt + state->brushSize;
    bool atCanvas = MouseIsAtCanvas(state, canvasRect);

    Vector2 cPos = {canvasPos.x, canvasPos.y};

    Color dClr = state->curTool == DT_ERASER ? WHITE : state->current;

    if (state->curTool == DT_PENCIL || state->curTool == DT_ERASER) {

        if (pxAtCanvas && IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
            !state->enablePanning) {
            if (state->brushSize >= 1) {
                if (state->brushSize == 1) {
                    ImageDrawPixel(&state->canvasImg, pl, pt, dClr);
                } else if (state->brushSize > 1) {
                    ImageDrawRectangle(
                        &state->canvasImg, pl, pt, state->brushSize,
                        state->brushSize, dClr
                    );
                }
                UpdateTexture(state->canvasTxt, state->canvasImg.data);
            }
        }
    } else if (state->curTool == DT_LINE) {
        if (pxAtCanvas && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->lineStart.x = pl;
            state->lineStart.y = pt;

            state->lineDragging = true;
        }

        if (pxAtCanvas && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (pl == state->lineStart.x && pt == state->lineStart.y) {
                ImageDrawPixel(&state->canvasImg, pl, pt, dClr);
                UpdateTexture(state->canvasTxt, state->canvasImg.data);

            } else {
                canvasPos = getCanvasPos(state, canvasRect);
                // ImageDrawLineEx(
                //     &state->canvasImg, state->lineStart,
                //     (Vector2){canvasPos.x, canvasPos.y}, state->brushSize,
                //     dClr
                //);
                BPDrawLine(
                    state, &state->canvasImg, state->lineStart,
                    (Vector2){pl, pt}, dClr
                );
                UpdateTexture(state->canvasTxt, state->canvasImg.data);
                ImageClearBackground(&state->previewImg, BLANK);

                ImageClearBackground(&state->previewImg, BLANK);
            }

            state->lineStart.x = -1;
            state->lineStart.y = -1;
            state->lineDragging = false;
        }
    }

    ImageClearBackground(&state->previewImg, BLANK);

    if (atCanvas && !state->enablePanning && state->curTool != DT_NOTOOL) {
        if (state->brushSize >= 1) {
            if (state->brushSize == 1) {
                ImageDrawPixel(&state->previewImg, pl, pt, dClr);
            } else if (state->brushSize > 1) {
                ImageDrawRectangle(
                    &state->previewImg, pl, pt, state->brushSize,
                    state->brushSize, dClr
                );
            }
        }
    }

    if (state->lineDragging) {
        /*ImageDrawLineEx(
            &state->previewImg, state->lineStart,
            (Vector2){canvasPos.x, canvasPos.y}, state->brushSize, dClr
        );
                */

        BPDrawLine(
            state, &state->previewImg, state->lineStart, (Vector2){pl, pt}, dClr
        );

        // float radAngle = GetRadAngleAtoB(state->lineStart, cPos);
        // float degAngle = GetDegAngleAtoB(state->lineStart, cPos);
        // TraceLog(LOG_ERROR, "Line Angle D[%f] | R[%f]", degAngle, radAngle);
    }

    DrawTexture(state->canvasTxt, state->drawArea.x, state->drawArea.y, WHITE);
    UpdateTexture(state->previewTxt, state->previewImg.data);
    DrawTexture(state->previewTxt, state->drawArea.x, state->drawArea.y, WHITE);
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
    switch (state->curTool) {
    case DT_PENCIL: {
        if (atCanvas) {
            SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
        }
        break;
    }

    case DT_PAN: {
        state->enablePanning = true;
        if (atDrawArea) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }

        break;
    }

    default: {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    }
}

bool Canvas(CanvasState *state) {
    updateBounds(state);
    handleTools(state);

    DrawRectangleRounded(
        state->prop.bounds, DA_ROUNDNESS, 0, ColorFDGrayLighter
    );
    Vector2 mpos = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mpos, state->drawArea);

    Vector2 drawVector = (Vector2){state->drawArea.x, state->drawArea.y};
    Rectangle canvasRect = {
        state->drawArea.x, state->drawArea.y, state->gridSize.x,
        state->gridSize.y
    };

    Vector2 canvasVector = {canvasRect.x, canvasRect.y};

    if (isHovering) {
        float wheel = GetMouseWheelMove();

        if (wheel != 0) {
            Vector2 mwPos =
                GetScreenToWorld2D(GetMousePosition(), state->camera);
            state->camera.offset = GetMousePosition();
            state->camera.target = mwPos;
            state->point = mwPos;
            float scale = 0.2f * wheel;
            state->camera.zoom =
                Clamp(expf(logf(state->camera.zoom) + scale), 0.125f, 64.0f);
        }

        if (state->enablePanning && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

            Vector2 delta =
                Vector2Scale(GetMouseDelta(), -1.0 / state->camera.zoom);

            state->point = Vector2Add(state->point, delta);
            state->camera.target = state->point;

            state->panning = true;
        }
    }

    if (IsKeyReleased(KEY_LEFT_SHIFT) ||
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        state->panning = false;
    }

    Vector2 dir = {0, 0};
    /*

if (IsKeyDown(KEY_LEFT))
    dir.x++;
if (IsKeyDown(KEY_RIGHT))
    dir.x--;
if (IsKeyDown(KEY_UP))
    dir.y++;
if (IsKeyDown(KEY_DOWN))
    dir.y--;
    */
    bool movingKb = false;
    if (Vector2Length(dir) != 0.0f) {
        movingKb = true;

        dir = Vector2Scale(Vector2Normalize(dir), 200.0f * GetFrameTime());
        state->point = Vector2Add(state->point, dir);
    }

    Vector2 dTl = GetScreenToWorld2D(drawVector, state->camera);
    Vector2 dBr = GetScreenToWorld2D(
        (Vector2){drawVector.x + state->drawArea.width,
                  state->drawArea.y + state->drawArea.height},
        state->camera
    );

    Vector2 cTl = canvasVector;
    Vector2 cBr = {
        canvasVector.x + canvasRect.width, canvasVector.y + canvasRect.height
    };

    float leftEmpty = cTl.x - dTl.x;
    float rightEmpty = dBr.x - cBr.x;

    float topEmpty = cTl.y - dTl.y;
    float bottomEmpty = dBr.y - cBr.y;

    float mv = 200.0f * GetFrameTime();
    Vector4 drw = {
        .x = dTl.x,
        .y = dTl.y,
        .z = dBr.x,
        .w = dBr.y,
    };
    Vector4 cvs = {
        .x = cTl.x,
        .y = cTl.y,
        .z = cBr.x,
        .w = cBr.y,
    };

    CanvasScrollBars(state, drw, cvs);

    if (state->panning || movingKb) {
        float leftOutside = (canvasRect.x + canvasRect.width) - dTl.x;
        float rightOutside = canvasRect.x - dBr.x;
        float topOutside = (canvasRect.y + canvasRect.height) - dTl.y;
        float bottomOutside = canvasRect.y - dBr.y;
        if (leftOutside < 0) {
            state->point.x += leftOutside;
        } else if (topOutside < 0) {
            state->point.y += topOutside;
        } else if (rightOutside > 0) {
            state->point.x += rightOutside;
        } else if (bottomOutside > 0) {
            state->point.y += bottomOutside;
        }

        state->vScrollDragging = false;
        state->hScrollDragging = false;
    }

    BeginScissorMode(
        state->drawArea.x, state->drawArea.y, state->drawArea.width,
        state->drawArea.height
    );
    {

        // DrawRectangleRec(state->drawArea, ColorGrayLighter);
        GuiGrid(state->drawArea, NULL, state->gridSize.x * 2.0f, 1, NULL);
        BeginMode2D(state->camera);
        {
            DrawingCanvas(state, canvasRect);
            // DrawRectangleLinesEx(canvasRect, 0.1, BLACK);

            // DrawRectangleLinesEx(state->drawArea, 2, MAROON);
            // DrawCircleV(canvasVector, 10, ColorRedDark);
        }
        EndMode2D();
    }
    EndScissorMode();

    DrawRectangleRoundedLinesEx(
        state->prop.bounds, DA_ROUNDNESS, 0, 3, ColorBlack
    );

    DrawRectangleRoundedLinesEx(
        (Rectangle){
            state->prop.bounds.x + 2, state->prop.bounds.y + 2,
            state->prop.bounds.width - 4, state->prop.bounds.height - 4

        },
        DA_ROUNDNESS, 0, 2, ColorGrayLightest
    );

    state->camera.target.x = state->point.x;
    state->camera.target.y = state->point.y;

    return false;
}
