#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../external/raylib.h"
#include "../include/widgets/canvas.h"

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

static void BPDrawRectangle(
    CanvasState *state, Image *img, Vector2 a, Vector2 b, Color clr, bool fill
) {
    float xMin = fminf(a.x, b.x);
    float yMin = fminf(a.y, b.y);

    float xMax = fmaxf(a.x, b.x);
    float yMax = fmaxf(a.y, b.y);

    Vector2 topLeft = {xMin, yMin};
    Vector2 topRight = {xMax, yMin};
    Vector2 bottomRight = {xMax, yMax};
    Vector2 bottomLeft = {xMin, yMax};

    BPDrawLine(state, img, topLeft, topRight, clr);
    BPDrawLine(state, img, topRight, bottomRight, clr);
    BPDrawLine(state, img, bottomRight, bottomLeft, clr);
    BPDrawLine(state, img, bottomLeft, topLeft, clr);

    if (fill) {
        ImageDrawRectangle(img, xMin, yMin, xMax - xMin, yMax - yMin, clr);
    }
}

static bool rectDragging = false;

void DrawingCanvas(CanvasState *state, Rectangle bounds) {
    Rectangle canvasRect = (Rectangle){state->drawArea.x, state->drawArea.y,
                                       state->gridSize.x, state->gridSize.y};
    Vector4 canvasPos = getCanvasPos(state, canvasRect);

    bool pxAtCanvas = validCanvasPos(state->gridSize, canvasPos);

    int canPx = (int)canvasPos.x;
    int canPy = (int)canvasPos.y;

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
    } else if (state->curTool == DT_RECT || state->curTool == DT_RECT_FILL) {
        if (pxAtCanvas && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->lineStart.x = pl;
            state->lineStart.y = pt;

            rectDragging = true;
        }

        if (pxAtCanvas && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (pl == state->lineStart.x && pt == state->lineStart.y) {
                ImageDrawPixel(&state->canvasImg, pl, pt, dClr);
                UpdateTexture(state->canvasTxt, state->canvasImg.data);
            } else {
                canvasPos = getCanvasPos(state, canvasRect);

                BPDrawRectangle(
                    state, &state->canvasImg, state->lineStart,
                    (Vector2){canvasPos.x, canvasPos.y}, dClr,
                    state->curTool == DT_RECT_FILL
                );

                UpdateTexture(state->canvasTxt, state->canvasImg.data);
                ImageClearBackground(&state->previewImg, BLANK);

                ImageClearBackground(&state->previewImg, BLANK);
            }

            state->lineStart.x = -1;
            state->lineStart.y = -1;
            rectDragging = false;
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
        BPDrawLine(
            state, &state->previewImg, state->lineStart, (Vector2){pl, pt}, dClr
        );

        // float radAngle = GetRadAngleAtoB(state->lineStart, cPos);
        // float degAngle = GetDegAngleAtoB(state->lineStart, cPos);
        // TraceLog(LOG_ERROR, "Line Angle D[%f] | R[%f]", degAngle, radAngle);
    }

    if (rectDragging) {

        canvasPos = getCanvasPos(state, canvasRect);
        BPDrawRectangle(
            state, &state->previewImg, state->lineStart,
            (Vector2){canvasPos.x, canvasPos.y}, dClr,
            state->curTool == DT_RECT_FILL
        );
    }

    DrawTexture(state->canvasTxt, state->drawArea.x, state->drawArea.y, WHITE);
    UpdateTexture(state->previewTxt, state->previewImg.data);
    DrawTexture(state->previewTxt, state->drawArea.x, state->drawArea.y, WHITE);
}
