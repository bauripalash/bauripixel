#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/utils.h"
#include "../include/widgets/canvas.h"

static void
DrawBrush(CanvasState *state, Image *img, int posX, int posY, Color clr);

static void
BPDrawCircle(Image *img, int posX, int posY, int r, Color clr, bool fill);

// Returns position of mouse if inside a drawArea
// x,y => canvas cord
Vector2 getCanvasPos(CanvasState *state, Rectangle canvasRect) {
    Vector2 mpos = GetMousePosition();
    Vector2 wMpos = GetScreenToWorld2D(mpos, state->camera);

    if (CheckCollisionPointRec(mpos, state->drawArea)) {
        float px = floorf(wMpos.x - canvasRect.x);
        float py = floorf(wMpos.y - canvasRect.y);
        return (Vector2){px, py};
    }
    return (Vector2){-1, -1};
}

bool validCanvasPos(Vector2 gridSize, Vector2 pos) {
    if ((pos.x < 0 || pos.x >= gridSize.x) ||
        (pos.y < 0 || pos.y >= gridSize.y)) {
        return false;
    }
    return true;
}

bool MouseIsAtRect(Rectangle rect) {
    return (CheckCollisionPointRec(GetMousePosition(), rect));
}

bool MouseIsAtCanvas(CanvasState *state, Rectangle rect) {
    Vector2 mpos = GetMousePosition();
    return (
        CheckCollisionPointRec(mpos, state->drawArea) &&
        CheckCollisionPointRec(GetScreenToWorld2D(mpos, state->camera), rect)
    );
}

static inline void drawHLine(Image *img, int x0, int x1, int y, Color clr) {
    ImageDrawLine(img, x0, y, x1, y, clr);
}
// Bresenham's line algorithm
// based on https://zingl.github.io/bresenham.html
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
    int e2 = 0;

    while (true) {
        DrawBrush(state, img, x0, y0, clr);
        e2 = 2 * err;

        if (x0 == x1 && y0 == y1) {
            break;
        }

        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

static void BPDrawRectangle(
    CanvasState *state, Image *img, Vector2 a, Vector2 b, Color clr, bool fill
) {
    Vector2 start = a;
    Vector2 end = b;

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
        float dx = end.x - start.x;
        float dy = end.y - start.y;
        float len = fabsf(dx);
        float signX = dx >= 0 ? 1.0f : -1.0f;
        float signY = (dy >= 0) ? 1.0f : -1.0f;

        end.x = start.x + signX * len;
        end.y = start.y + signY * len;
    }

    float xMin = fminf(start.x, end.x);
    float yMin = fminf(start.y, end.y);

    float xMax = fmaxf(start.x, end.x);
    float yMax = fmaxf(start.y, end.y);

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

// Based on https://zingl.github.io/bresenham.html
static void ellipseInRect(
    CanvasState *state, Image *img, Vector2 a, Vector2 b, Color clr, bool fill
) {
    int brushSize = state->brushSize;
    int x0 = (int)a.x;
    int y0 = (int)a.y;

    int x1 = (int)b.x;
    int y1 = (int)b.y;

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int len = abs(dx);
        int signX = dx >= 0 ? 1 : -1;
        int signY = dy >= 0 ? 1 : -1;

        x1 = x0 + signX * len;
        y1 = y0 + signY * len;
    }

    long _a = abs(x1 - x0);
    long _b = abs(y1 - y0);
    long _b1 = _b & 1;
    double dx = 4 * (1 - _a) * _b * _b;
    double dy = 4 * (_b1 + 1) * _a * _a;
    double err = dx + dy + _b1 * _a * _a;
    double e2 = 0;

    if (x0 > x1) {
        x0 = x1;
        x1 += _a;
    }

    if (y0 > y1) {
        y0 = y1;
    }

    y0 += (_b + 1) / 2;
    y1 = y0 - _b1;
    _a *= 8 * _a;
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
        TraceLog(
            LOG_ERROR, "[X0 %d] [Y0 %d] | [X1 %d] [Y1 %d]", x0, y0, x1, y1
        );
        TraceLog(LOG_ERROR, "[_A %ld] [_B %ld] [_B1 %ld]", _a, _b, _b1);
        TraceLog(LOG_ERROR, "[DX %lf] [DY %lf]", dx, dy);
        TraceLog(LOG_ERROR, "[ERR %lf] [E2 %lf]", err, e2);
    } while (x0 <= x1);

    while (y0 - y1 < _b) {
        ImageDrawRectangle(img, x0 - 1, y0, brushSize, brushSize, clr);
        ImageDrawRectangle(img, x1 + 1, y0, brushSize, brushSize, clr);
        ImageDrawRectangle(img, x0 - 1, y1, brushSize, brushSize, clr);
        ImageDrawRectangle(img, x1 + 1, y1, brushSize, brushSize, clr);

        if (fill) {
            drawHLine(img, x0 - 1, x1 + 1, y0, clr);
            drawHLine(img, x0 - 1, x1 + 1, y1, clr);
        }

        y0++;
        y1--;
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

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
        int r = rx > ry ? rx : ry;
        rx = ry = r;
    }

    Vector2 xy0 = {xc - rx, yc - ry};

    Vector2 xy1 = {xc + rx, yc + ry};

    ellipseInRect(state, img, xy0, xy1, clr, fill);
}

static void BPDrawEllipse(
    CanvasState *state, Image *img, Vector2 a, Vector2 b, Color clr, bool fill,
    bool center
) {
    if (center) {
        ellipseInCenter(state, img, a, b, clr, fill);
    } else {
        ellipseInRect(state, img, a, b, clr, fill);
    }
}

static void
BPDrawCircle(Image *img, int posX, int posY, int r, Color clr, bool fill) {
    int x = -r;
    int y = 0;
    int err = 2 - 2 * r;

    do {
        ImageDrawPixel(img, posX - x, posY + y, clr);
        ImageDrawPixel(img, posX - y, posY - x, clr);
        ImageDrawPixel(img, posX + x, posY - y, clr);
        ImageDrawPixel(img, posX + y, posY + x, clr);

        int lx = posX + x;
        int rx = posX - x;

        int ty = posY + y;
        int by = posY - y;
        if (fill) {
            drawHLine(img, lx, rx, ty, clr);
            drawHLine(img, lx, rx, by, clr);
        }

        r = err;
        if (r <= y) {
            err += ++y * 2 + 1;
        }
        if (r > x || err > y) {
            err += ++x * 2 + 1;
        }
    } while (x < 0);
}

static void
DrawBrush(CanvasState *state, Image *img, int posX, int posY, Color clr) {
    int size = state->brushSize;
    if (state->brushShape == BSP_SQAURE) {
        ImageDrawRectangle(img, posX, posY, size, size, clr);
    } else {
        BPDrawCircle(img, posX, posY, size / 2, clr, true);
    }
}

void DrawingCanvas(CanvasState *state, Rectangle bounds) {

    bool locked = GuiIsLocked();
    Rectangle canvasRect = (Rectangle){state->drawArea.x, state->drawArea.y,
                                       state->gridSize.x, state->gridSize.y};

    // should not be used directly in drawing
    Vector2 rawCanvasPos = getCanvasPos(state, canvasRect);

    bool pxIsValid = validCanvasPos(state->gridSize, rawCanvasPos);
    bool atDrawArea = MouseIsAtRect(state->drawArea);
    bool atCanvas = atDrawArea && pxIsValid;

    int posLeft = (int)ceilf(rawCanvasPos.x - state->brushSize * 0.5f);
    int posTop = (int)ceilf(rawCanvasPos.y - state->brushSize * 0.5f);
    int posRight = posLeft + state->brushSize;
    int posBottom = posTop + state->brushSize;

    BrushShape shape = state->brushShape;

    int curPx = shape == BSP_SQAURE ? posLeft : (rawCanvasPos.x);
    int curPy = shape == BSP_SQAURE ? posTop : (rawCanvasPos.y);

    if (atDrawArea) {
        state->hoverX = curPx;
        state->hoverY = curPy;
    }

    // This is the actual current position accordingto brush size;
    Vector2 curPos = {curPx, curPy};

    Color dClr = state->curTool == DT_ERASER ? state->bgColor : state->current;

    DrawTool curtool = state->curTool;

    int brushSize = state->brushSize;

    bool leftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !locked;
    bool leftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !locked;
    bool leftReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !locked;
    bool keepRatio =
        IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) && !locked;

    Image *canvas = &state->canvasImg;
    Image *preview = &state->previewImg;

    if (!locked && atDrawArea) {
        switch (curtool) {
        case DT_PENCIL: {
            if (leftDown) {
                DrawBrush(state, canvas, curPx, curPy, dClr);
                UpdateTexture(state->canvasTxt, canvas->data);
            }
            break;
        }

        case DT_ERASER: {
            if (leftDown) {
                DrawBrush(state, canvas, curPx, curPy, dClr);
                UpdateTexture(state->canvasTxt, canvas->data);
            }
            break;
        }
        case DT_LINE: {
            if (leftPressed) {
                state->lineStart.x = curPx;
                state->lineStart.y = curPy;
                state->lineDragging = true;
            }

            if (leftReleased && state->lineDragging) {
                BPDrawLine(state, canvas, state->lineStart, curPos, dClr);
                UpdateTexture(state->canvasTxt, canvas->data);
                MakeVecZero(&state->lineStart);
                state->lineDragging = false;
            }
            break;
        }
        case DT_RECT:
        case DT_RECT_FILL: {
            if (leftPressed) {
                state->rectStart.x = curPx;
                state->rectStart.y = curPy;
                state->rectDragging = true;
            }

            if (leftReleased && state->rectDragging) {
                BPDrawRectangle(
                    state, canvas, state->rectStart, curPos, dClr,
                    curtool == DT_RECT_FILL
                );
                UpdateTexture(state->canvasTxt, canvas->data);
                MakeVecZero(&state->rectStart);
                state->rectDragging = false;
            }

            break;
        }
        case DT_CIRCLE:
        case DT_CIRCLE_FILL: {
            if (leftPressed) {
                state->circleStart.x = curPx;
                state->circleStart.y = curPy;
                state->circleDragging = true;
            }

            if (leftReleased && state->circleDragging) {
                BPDrawEllipse(
                    state, canvas, state->circleStart, curPos, dClr,
                    curtool == DT_CIRCLE_FILL, keepRatio
                );
                UpdateTexture(state->canvasTxt, canvas->data);
                MakeVecZero(&state->circleStart);
                state->circleDragging = false;
            }

            break;
        }

        default:
            break;
        }
    }

    ImageClearBackground(&state->previewImg, BLANK);

    if (atCanvas && !locked) {

        switch (curtool) {
        case DT_PENCIL: {
            DrawBrush(state, &state->previewImg, curPx, curPy, dClr);
            break;
        }
        case DT_ERASER: {
            DrawBrush(state, &state->previewImg, curPx, curPy, WHITE);
            break;
        }
        case DT_LINE: {
            if (!state->lineDragging) {
                DrawBrush(state, &state->previewImg, curPx, curPy, dClr);

            } else {
                BPDrawLine(
                    state, &state->previewImg, state->lineStart,
                    (Vector2){curPx, curPy}, dClr
                );
            }
            break;
        }

        case DT_RECT:
        case DT_RECT_FILL: {
            if (!state->rectDragging) {
                DrawBrush(state, &state->previewImg, curPx, curPy, dClr);
            } else {
                bool fill = curtool == DT_RECT_FILL;
                BPDrawRectangle(
                    state, &state->previewImg, state->rectStart, curPos, dClr,
                    fill
                );
            }
            break;
        }

        case DT_CIRCLE:
        case DT_CIRCLE_FILL: {
            if (!state->circleDragging) {
                DrawBrush(state, &state->previewImg, curPx, curPy, dClr);
            } else {
                bool keepRatio =
                    IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
                bool fill = curtool == DT_CIRCLE_FILL;
                BPDrawEllipse(
                    state, &state->previewImg, state->circleStart, curPos, dClr,
                    fill, keepRatio
                );
            }

            break;
        }

        default:
            break;
        }
    }

    DrawTexture(state->bgTxt, state->drawArea.x, state->drawArea.y, WHITE);
    DrawTexture(state->canvasTxt, state->drawArea.x, state->drawArea.y, WHITE);
    UpdateTexture(state->previewTxt, state->previewImg.data);
    DrawTexture(state->previewTxt, state->drawArea.x, state->drawArea.y, WHITE);
}
