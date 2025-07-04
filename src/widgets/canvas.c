#include "../include/widgets/canvas.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include <stdbool.h>
#include <stddef.h>

CanvasState NewCanvas() {
    CanvasState c = {0};
    c.prop = NewWidgetProp();
    c.hoverX = 0;
    c.hoverY = 0;
    c.gridSize = (Vector2){DEFAULT_GRID_SIZE, DEFAULT_GRID_SIZE};
    c.anchor = (Vector2){0, 0};
    c.bottomAnchor = (Vector2){0, 0};

    c.scroll = (Vector2){0, 0};
    c.zoomMin = 0.2f;
    c.zoomMax = INIT_CELL_SIZE;

    for (int i = 0; i < (int)c.gridSize.y; i++) {
        for (int j = 0; j < (int)c.gridSize.x; j++) {
            c.colors[i][j] = BLANK;
        }
    }

    c.current = ColorBlack;
    c.pxSize = INIT_CELL_SIZE;
    c.prop.bounds.x = CANVAS_MARGIN_L;
    c.prop.bounds.y = CANVAS_MARGIN_TB;
    c.prop.bounds.width =
        GetScreenWidth() - (CANVAS_MARGIN_L + CANVAS_MARGIN_R);
    c.prop.bounds.height = GetScreenHeight() - (CANVAS_MARGIN_TB * 2);

    c.point = (Vector2){0, 0};
    c.camera = (Camera2D){0};
    c.camera.zoom = 1.0f;

    Rectangle bounds = c.prop.bounds;
    c.drawArea = (Rectangle){bounds.x + CANVAS_DRAW_MARGIN,
                             bounds.y + CANVAS_DRAW_MARGIN,
                             bounds.width - (CANVAS_DRAW_MARGIN * 2),
                             bounds.height - (CANVAS_DRAW_MARGIN * 2)};

    c.hScrollRect = (Rectangle){c.drawArea.x, c.drawArea.y + c.drawArea.height,
                                c.drawArea.width, 20};

    c.vScrollRect = (Rectangle){c.drawArea.x + c.drawArea.width, c.drawArea.y,
                                20, c.drawArea.height};

    c.vScrollDragging = false;
    c.hScrollDragging = false;
    c.panning = false;
    c.enablePanning = false;

    c.camera.target = (Vector2){0};

    Vector2 canvasSize = {c.gridSize.x * c.pxSize, c.gridSize.y * c.pxSize};

    c.camera.offset =
        (Vector2){(c.drawArea.width / 2.0f) - (canvasSize.x / 2.0f),
                  (c.drawArea.height / 2.0f) - (canvasSize.y / 2.0f)};

    return c;
}

void updateBounds(CanvasState *c) {
    c->prop.bounds.x = c->anchor.x + CANVAS_MARGIN_L;
    c->prop.bounds.y = c->anchor.y + CANVAS_MARGIN_TB;
    c->prop.bounds.width = GetScreenWidth() -
                           (CANVAS_MARGIN_L + CANVAS_MARGIN_R) - c->anchor.x -
                           c->bottomAnchor.x - c->vScrollRect.width;
    c->prop.bounds.height = GetScreenHeight() - (CANVAS_MARGIN_TB * 2) -
                            c->anchor.y - c->bottomAnchor.y -
                            c->hScrollRect.height;

    Rectangle bounds = c->prop.bounds;
    c->drawArea = (Rectangle){bounds.x + CANVAS_DRAW_MARGIN,
                              bounds.y + CANVAS_DRAW_MARGIN,
                              bounds.width - (CANVAS_DRAW_MARGIN * 2),
                              bounds.height - (CANVAS_DRAW_MARGIN * 2)};

    c->vScrollRect = (Rectangle){c->drawArea.x + c->drawArea.width,
                                 c->drawArea.y, 20, c->drawArea.height};

    c->hScrollRect =
        (Rectangle){c->drawArea.x, c->drawArea.y + c->drawArea.height,
                    c->drawArea.width, 20};
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
}

void CenterAlignCanvas(CanvasState *state) {

    Vector2 canvasSize = {
        state->gridSize.x * state->pxSize, state->gridSize.y * state->pxSize
    };

    state->camera.offset =
        (Vector2){(state->drawArea.width / 2.0f) - (canvasSize.x / 2.0f),
                  (state->drawArea.height / 2.0f) - (canvasSize.y / 2.0f)};
}

void SetCurrentCanvasColor(CanvasState *state, Color color) {
    state->current = color;
}

void TraceVector(Vector2 vec, const char *string) {
    TraceLog(LOG_WARNING, "%s [%f, %f]", string, vec.x, vec.y);
}

void TraceRect(Rectangle rect, const char *string) {
    TraceLog(
        LOG_WARNING, "%s [[%f, %f],[%f, %f]]", string, rect.x, rect.y,
        rect.width, rect.height
    );
}

void drawBorder(CanvasState *state) {
    DrawRectangleRec(state->drawArea, ColorWhite);
    DrawRectangleLinesEx(state->drawArea, 1, ColorGrayLighter);
}

#define TMR 3
#define TMW 50

static bool vDragging = false;
static float vDispl = 0.0f;
#define SCROLL_ROUNDNESS 0.9

bool CanvasScrollBars(CanvasState *state, Vector4 drawArea, Vector4 canvas) {
    Rectangle vBounds = state->vScrollRect;
    Rectangle hBounds = state->hScrollRect;
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

    DrawRectangleRounded(vBounds, SCROLL_ROUNDNESS, 0, ColorGreenLightest);
    DrawRectangleRounded(hBounds, SCROLL_ROUNDNESS, 0, ColorGreenLightest);
    DrawRectangleRounded(hThumbRect, SCROLL_ROUNDNESS, 0, ColorGreenDarkest);
    DrawRectangleRounded(vThumbRect, SCROLL_ROUNDNESS, 0, ColorGreenDarkest);

    if (CheckCollisionPointRec(GetMousePosition(), hBounds)) {
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

void DrawingGrid(CanvasState *state, Rectangle bounds) {

    int maxRow = (int)state->gridSize.y;
    int maxColumn = (int)state->gridSize.x;
    float pxSize = state->pxSize;

    for (int row = 0; row < maxRow; row++) {
        for (int col = 0; col < maxColumn; col++) {
            Rectangle rect = {
                bounds.x + ((float)col * pxSize),
                bounds.y + ((float)row * pxSize), pxSize, pxSize
            };

            if (!state->panning && cellClicked(rect, state->camera)) {
                state->colors[row][col] = state->current;
            }

            DrawRectangleRec(rect, state->colors[row][col]);
            DrawRectangleLinesEx(rect, 0.5, Fade(ColorGrayLighter, 0.5));
        }
    }
}

bool Canvas(CanvasState *state) {
    updateBounds(state);
    Vector2 mpos = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mpos, state->drawArea);

    Vector2 drawVector = (Vector2){state->drawArea.x, state->drawArea.y};
    Rectangle canvasRect = {
        state->drawArea.x, state->drawArea.y,
        (state->gridSize.x * state->pxSize), (state->gridSize.y * state->pxSize)
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
            state->camera.zoom += scale;
            state->camera.zoom = Clamp(state->camera.zoom, 0.2f, 10.0f);

            Vector2 center = {
                (state->drawArea.x + state->drawArea.width) / 2.0f,
                (state->drawArea.y + state->drawArea.height) / 2.0f
            };
            center = GetScreenToWorld2D(center, state->camera);
        }

        if (state->enablePanning && IsKeyDown(KEY_LEFT_SHIFT) &&
            IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

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

    if (IsKeyDown(KEY_LEFT))
        dir.x++;
    if (IsKeyDown(KEY_RIGHT))
        dir.x--;
    if (IsKeyDown(KEY_UP))
        dir.y++;
    if (IsKeyDown(KEY_DOWN))
        dir.y--;

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

    Rectangle sRect = {
        state->drawArea.x, state->drawArea.y + state->drawArea.height,
        state->drawArea.width, 20
    };

    state->vScrollRect =
        (Rectangle){state->drawArea.x + state->drawArea.width,
                    state->drawArea.y, 20, state->drawArea.height};

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

    drawBorder(state);
    BeginScissorMode(
        state->drawArea.x, state->drawArea.y, state->drawArea.width,
        state->drawArea.height
    );
    {
        GuiGrid(state->drawArea, NULL, state->gridSize.x * 2, 2, NULL);
        BeginMode2D(state->camera);
        {
            DrawingGrid(state, canvasRect);
            // DrawCircleV(dTl, 30, ColorRedDark);
            // DrawCircleV(cTl, 30, ColorYellow);
            // DrawLineEx(dTl, cTl, 3, ColorOrange);
            // DrawCircleV(dBr, 30, ColorGreenLighter);
            // DrawCircleV(cBr, 30, ColorBlueLighter);
            // DrawLineEx(dBr, cBr, 3, ColorOrange);
            DrawRectangleLinesEx(canvasRect, 2, ColorBlack);
        }
        EndMode2D();
    }
    EndScissorMode();

    // TraceLog(
    //     LOG_WARNING, "VIEWPORT [%f] | C[%f] | Z[%f]", dBr.x - dTl.x,
    //     (cBr.x - cTl.x) * state->camera.zoom, state->camera.zoom
    //);
    //  BauScroll(sRect, &sPoint, move, leftEmpty, rightEmpty, dBr.x - dTl.x);

    state->camera.target.x = state->point.x;
    state->camera.target.y = state->point.y;
    // TraceLog(LOG_WARNING, "--------------------");

    return false;
}
