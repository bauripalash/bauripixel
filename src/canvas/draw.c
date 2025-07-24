#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include "../include/widgets/canvas.h"

// Returns position of mouse if inside a canvas.
// x,y => canvas cord || w,z => worldMousePos
Vector4 getCanvasPos(CanvasState *state, Rectangle canvasRect) {
    Vector2 mpos = GetMousePosition();
    Vector2 wMpos = GetScreenToWorld2D(mpos, state->camera);

    if (CheckCollisionPointRec(mpos, state->drawArea)) {
        // if (CheckCollisionPointRec(wMpos, canvasRect)) {
        float px = floorf(wMpos.x - canvasRect.x);
        float py = floorf(wMpos.y - canvasRect.y);

        return (Vector4){px, py, wMpos.x, wMpos.y};
        //}
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

static void drawHLine(Image *img, int x0, int x1, int y, Color clr) {
    ImageDrawLine(img, x0, y, x1, y, clr);
}

// Based on https://zingl.github.io/bresenham.html
static void ellipseInRect(
    CanvasState *state, Image *img, Vector2 a, Vector2 b, Color clr, bool fill
) {
    int brushSize = state->brushSize;
    int x0 = (int)a.x;
    int y0 = (int)a.y;

    int x1 = (int)b.x;
    int y1 = (int)b.y;

    int _a = abs(x1 - x0);
    int _b = abs(y1 - y0);
    int _b1 = _b & 1;
    long dx = 4 * (1 - _a) * _b * _b;
    long dy = 4 * (_b1 + 1) * _a * _a;
    long err = dx + dy + _b1 * _a * _a;
    long e2 = 0;

    if (x0 > x1) {
        x0 = x1;
        x1 += _a;
    }

    if (y0 > y1) {
        y0 = y1;
    }

    y0 += (_b + 1) / 2;
    y1 = y0 - _b1;
    _a = 8 * _a * _a;
    _b1 = 8 * _b * _b;

    do {
        ImageDrawRectangle(img, x1, y0, brushSize, brushSize, clr);
        ImageDrawRectangle(img, x0, y0, brushSize, brushSize, clr);
        ImageDrawRectangle(img, x0, y1, brushSize, brushSize, clr);
        ImageDrawRectangle(img, x1, y1, brushSize, brushSize, clr);

        if (fill) {
            drawHLine(img, x0, x1, y0, clr);
            drawHLine(img, x0, x1, y1, clr);
        }

        e2 = 2 * err;
        if (e2 <= dy) {
            y0++;
            y1--;
            err += dy += _a;
        }

        if (e2 >= dx || 2 * err > dy) {
            x0++;
            x1--;

            err += dx += _b1;
        }
    } while (x0 <= x1);

    while (y0 - y1 < _b) {
        ImageDrawRectangle(img, x0 - 1, y0, brushSize, brushSize, clr);
        ImageDrawRectangle(img, x1 + 1, y0++, brushSize, brushSize, clr);
        ImageDrawRectangle(img, x0 - 1, y1, brushSize, brushSize, clr);
        ImageDrawRectangle(img, x1 + 1, y1--, brushSize, brushSize, clr);

        if (fill) {
            drawHLine(img, x0 - 1, x1 + 1, y0, clr);
            drawHLine(img, x0 - 1, x1 + 1, y1, clr);
        }
    }
}

static void ellipseInCenter(
    CanvasState *state, Image *img, Vector2 a, Vector2 b, Color clr, bool fill
) {
    int xc = (int)a.x; // center x
    int yc = (int)a.y; // center y

    int xd = (int)b.x;
    int yd = (int)b.y;

    int rx = abs(xd - xc);
    int ry = abs(yd - yc);

    Vector2 xy0 = {xc - rx, yc - ry};

    Vector2 xy1 = {xc + rx, yc + ry};

    ellipseInRect(state, img, xy0, xy1, clr, fill);
}

static void BPDrawCircle(
    CanvasState *state, Image *img, Vector2 a, Vector2 b, Color clr, bool fill,
    bool center
) {

    if (center) {
        ellipseInCenter(state, img, a, b, clr, fill);
    } else {
        ellipseInRect(state, img, a, b, clr, fill);
    }
}

static bool rectDragging = false;
static bool circleDragging = false;

void DrawingCanvas(CanvasState *state, Rectangle bounds) {
    Rectangle canvasRect = (Rectangle){state->drawArea.x, state->drawArea.y,
                                       state->gridSize.x, state->gridSize.y};
    Vector4 canvasPos = getCanvasPos(state, canvasRect);

    bool pxAtCanvas = validCanvasPos(state->gridSize, canvasPos);
    Vector2 mpos = GetMousePosition();

    int canPx = (int)canvasPos.x;
    int canPy = (int)canvasPos.y;

    int pl = (int)ceilf(canvasPos.x - state->brushSize * 0.5f);
    int pt = (int)ceilf(canvasPos.y - state->brushSize * 0.5f);
    int pr = pl + state->brushSize;
    int pb = pt + state->brushSize;
    bool atCanvas = MouseIsAtCanvas(state, canvasRect);

    Vector2 cPos = {canvasPos.x, canvasPos.y};

    Color dClr = state->curTool == DT_ERASER ? WHITE : state->current;

    DrawTool curtool = state->curTool;

    if (state->curTool == DT_PENCIL || state->curTool == DT_ERASER) {

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !state->enablePanning) {
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
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->lineStart.x = pl;
            state->lineStart.y = pt;

            state->lineDragging = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
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
            }

            // state->lineStart.x = -1;
            // state->lineStart.y = -1;
            state->lineStart.x = 0;
            state->lineStart.y = 0;
            state->lineDragging = false;
        }
    } else if (state->curTool == DT_RECT || state->curTool == DT_RECT_FILL) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->lineStart.x = pl;
            state->lineStart.y = pt;

            rectDragging = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
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
            }

            // state->lineStart.x = -1;
            // state->lineStart.y = -1;
            state->lineStart.x = 0;
            state->lineStart.y = 0;
            rectDragging = false;
        }
    } else if (state->curTool == DT_CIRCLE ||
               state->curTool == DT_CIRCLE_FILL) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->lineStart.x = pl;
            state->lineStart.y = pt;

            circleDragging = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (pl == state->lineStart.x && pt == state->lineStart.y) {
                ImageDrawPixel(&state->canvasImg, pl, pt, dClr);
                UpdateTexture(state->canvasTxt, state->canvasImg.data);
            } else {
                canvasPos = getCanvasPos(state, canvasRect);
                BPDrawCircle(
                    state, &state->canvasImg, state->lineStart,
                    (Vector2){canvasPos.x, canvasPos.y}, dClr,
                    curtool == DT_CIRCLE_FILL, IsKeyDown(KEY_LEFT_SHIFT)
                );
                UpdateTexture(state->canvasTxt, state->canvasImg.data);
                ImageClearBackground(&state->previewImg, BLANK);
            }
            state->lineStart.x = 0;
            state->lineStart.y = 0;
            circleDragging = false;
        }
    }

    ImageClearBackground(&state->previewImg, BLANK);

    // NOTE: what should eraser draw?
    if (atCanvas && !state->enablePanning && curtool != DT_NOTOOL) {
        if (state->brushSize >= 1) {
            if (state->brushSize == 1) {
                if (curtool == DT_ERASER) {
                    ImageDrawPixel(
                        &state->previewImg, pl, pt,
                        Fade(ColorGreenLightest, 0.3)
                    );
                } else {
                    ImageDrawPixel(&state->previewImg, pl, pt, dClr);
                }
            } else if (state->brushSize > 1) {
                if (curtool == DT_ERASER) {

                    ImageDrawRectangleLines(
                        &state->previewImg,
                        (Rectangle){pl, pt, state->brushSize, state->brushSize},
                        1, Fade(ColorGreenLightest, 0.3)
                    );
                } else {
                    ImageDrawRectangle(
                        &state->previewImg, pl, pt, state->brushSize,
                        state->brushSize, dClr
                    );
                }
            }
        }
    }

    if (curtool == DT_LINE && state->lineDragging) {
        BPDrawLine(
            state, &state->previewImg, state->lineStart, (Vector2){pl, pt}, dClr
        );

        // float radAngle = GetRadAngleAtoB(state->lineStart, cPos);
        // float degAngle = GetDegAngleAtoB(state->lineStart, cPos);
        // TraceLog(LOG_ERROR, "Line Angle D[%f] | R[%f]", degAngle, radAngle);
    }

    if ((curtool == DT_RECT || curtool == DT_RECT_FILL) && rectDragging) {
        canvasPos = getCanvasPos(state, canvasRect);
        BPDrawRectangle(
            state, &state->previewImg, state->lineStart,
            (Vector2){canvasPos.x, canvasPos.y}, dClr,
            state->curTool == DT_RECT_FILL
        );
    }

    if ((curtool == DT_CIRCLE_FILL || curtool == DT_CIRCLE) && circleDragging) {
        canvasPos = getCanvasPos(state, canvasRect);
        BPDrawCircle(
            state, &state->previewImg, state->lineStart,
            (Vector2){canvasPos.x, canvasPos.y}, dClr,
            curtool == DT_CIRCLE_FILL, IsKeyDown(KEY_LEFT_SHIFT)
        );
    }

    DrawTexture(state->canvasTxt, state->drawArea.x, state->drawArea.y, WHITE);
    UpdateTexture(state->previewTxt, state->previewImg.data);
    DrawTexture(state->previewTxt, state->drawArea.x, state->drawArea.y, WHITE);
}
