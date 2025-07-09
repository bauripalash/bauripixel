#include "../include/widgets/canvas.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include "../include/utils.h"
#include <stdbool.h>
#include <stddef.h>

static Rectangle trect = {0};

CanvasState NewCanvas() {
    CanvasState c = {0};
    c.prop = NewWidgetProp();
    c.hoverX = 0;
    c.hoverY = 0;
    c.gridSize = (Vector2){DEFAULT_GRID_SIZE, DEFAULT_GRID_SIZE};
    c.anchor = (Vector2){0, 0};
    c.bottomAnchor = (Vector2){0, 0};
    c.scrollBody = ColorBlack;
    c.scrollThumb = ColorBlueLighter;

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
    c.prop.bounds.width =
        GetScreenWidth() - (CANVAS_MARGIN_L + CANVAS_MARGIN_R);
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
    c.enablePanning = true;

    float zm = c.drawArea.width / c.gridSize.x / 2.0f;

    c.camera.target = (Vector2){0, 0};

    c.camera.zoom = zm;

    c.canvasImg = GenImageColor(c.gridSize.x, c.gridSize.y, WHITE);
    c.canvasTxt = LoadTextureFromImage(c.canvasImg);

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

    DrawRectangleRounded(vBounds, SCROLL_ROUNDNESS, 0, state->scrollBody);
    DrawRectangleRounded(hBounds, SCROLL_ROUNDNESS, 0, state->scrollBody);
    DrawRectangleRounded(hThumbRect, SCROLL_ROUNDNESS, 0, state->scrollThumb);
    DrawRectangleRounded(vThumbRect, SCROLL_ROUNDNESS, 0, state->scrollThumb);

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

#define G_GRID_X 8
#define G_GRID_Y 8

void DrawingCanvas(CanvasState *state, Rectangle bounds) {
    Rectangle canvasRect = (Rectangle){state->drawArea.x, state->drawArea.y,
                                       state->gridSize.x, state->gridSize.y};
    Vector2 mPos = GetMousePosition();
    Vector2 worldMPos = GetScreenToWorld2D(mPos, state->camera);
    int px = (int)worldMPos.x - (int)canvasRect.x;
    int py = (int)worldMPos.y - (int)canvasRect.y;

    bool pxAtCanvas = (px >= 0 && px < (int)state->gridSize.x) &&
                      (py >= 0 && py < (int)state->gridSize.y);
    // if (pxAtCanvas) SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

    if (pxAtCanvas && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        ImageDrawPixel(&state->canvasImg, px, py, state->current);
        UpdateTexture(state->canvasTxt, state->canvasImg.data);
        // UpdateTextureRec(state->canvasTxt, (Rectangle){px,py,px,py},
        // state->canvasImg.data); // FIX THIS
    }
    DrawTexture(state->canvasTxt, state->drawArea.x, state->drawArea.y, WHITE);

    if (pxAtCanvas) {
        DrawRectangleRec(
            (Rectangle){canvasRect.x + px, canvasRect.y + py, 1, 1},
            state->current
        );
    }

    // SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

bool Canvas(CanvasState *state) {
    updateBounds(state);

    DrawRectangleRounded(state->prop.bounds, DA_ROUNDNESS, 0, ColorGrayDarkest);
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
            // state->camera.zoom += scale;
            // state->camera.zoom = Clamp(state->camera.zoom, 0.2f, 10.0f);
            state->camera.zoom =
                Clamp(expf(logf(state->camera.zoom) + scale), 0.125f, 64.0f);
            // TraceLog(LOG_ERROR, "Zoom %f", state->camera.zoom);
        }

        if (state->enablePanning && IsKeyDown(KEY_LEFT_SHIFT) &&
            IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

            Vector2 delta =
                Vector2Scale(GetMouseDelta(), -1.0 / state->camera.zoom);

            state->point = Vector2Add(state->point, delta);
            state->camera.target = state->point;

            state->panning = true;

            // TraceVector(state->camera.offset, "OFFSET");
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

        DrawRectangleRec(state->drawArea, ColorGrayLighter);
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
        state->prop.bounds, DA_ROUNDNESS, 0, 2, ColorGrayLightest
    );

    state->camera.target.x = state->point.x;
    state->camera.target.y = state->point.y;

    return false;
}
