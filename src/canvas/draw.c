#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/stb/stb_ds.h"
#include "../include/colors.h"
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
        DrawBrush(state, img, x1, y0, clr);
        DrawBrush(state, img, x0, y0, clr);
        DrawBrush(state, img, x0, y1, clr);
        DrawBrush(state, img, x1, y1, clr);

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
        DrawBrush(state, img, x0 - 1, y0, clr);
        DrawBrush(state, img, x1 + 1, y0, clr);
        DrawBrush(state, img, x0 - 1, y1, clr);
        DrawBrush(state, img, x1 + 1, y1, clr);

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

static Color ImageGetPixel(Image *img, int posX, int posY) {
    return GetImageColor(*img, posX, posY);
}

typedef struct FillSeg {
    int x;
    int y;
} FillSeg;

// scanline flood fill
static void
BpFill(CanvasState *state, Image *img, int posX, int posY, Color fillClr) {
    int width = img->width;
    int height = img->height;
    int startX = posX;
    int startY = posY;

    Color targetColor = ImageGetPixel(img, posX, posY);
    if (ColorIsEqual(fillClr, targetColor)) {
        return;
    }

    FillSeg *stack = NULL;
    arrpush(stack, ((FillSeg){posX, posY}));
    int len = 1;
    while (len > 0) {
        FillSeg seg = arrpop(stack);
        int y = seg.y;
        int x = seg.x;
        int leftX = x;
        int rightX = x;

        while (leftX > 0 &&
               ColorIsEqual(ImageGetPixel(img, leftX - 1, y), targetColor)) {
            leftX--;
        }

        while (rightX < width - 1 &&
               ColorIsEqual(ImageGetPixel(img, rightX + 1, y), targetColor)) {
            rightX++;
        }
        for (int xi = leftX; xi <= rightX; xi++) {
            ImageDrawPixel(img, xi, y, fillClr);

            if (y > 0 &&
                ColorIsEqual(ImageGetPixel(img, xi, y - 1), targetColor)) {
                arrpush(stack, ((FillSeg){xi, y - 1}));
            }

            if (y < height - 1 &&
                ColorIsEqual(ImageGetPixel(img, xi, y + 1), targetColor)) {
                arrpush(stack, ((FillSeg){xi, y + 1}));
            }
        }
        len = arrlen(stack);
    }

    arrfree(stack);
}

static const Color EraserColor = {.r = 0xff, .g = 0xff, .b = 0xff, .a = 100};

void DrawingCanvasLogic(CanvasState *state, Rectangle bounds) {
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

    BrushShape shape = state->brushShape;

    DrawTool curtool = state->curTool;

    int curPx = rawCanvasPos.x;
    int curPy = rawCanvasPos.y;

    if (curtool != DT_BUCKET && (shape == BSP_SQAURE)) {
        curPx = posLeft;
        curPy = posTop;
    }

    if (atDrawArea) {
        state->hoverX = curPx;
        state->hoverY = curPy;
    }
    bool isCurVisible = state->curLayer->visible;
    // This is the actual current position accordingto brush size;
    Vector2 curPos = {curPx, curPy};
    Color dClr = state->curTool == DT_ERASER ? state->bgColor : state->current;

    bool leftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !locked;
    bool leftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !locked;
    bool leftReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !locked;
    bool keepRatio =
        (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && !locked;

    LayerObj *layer = state->curLayer;

    if (!locked && atDrawArea && isCurVisible) {
        switch (curtool) {
        case DT_PENCIL: {

            if (!state->brushDragging && IsKeyDown(KEY_LEFT_SHIFT) &&
                !IsVecNeg(state->lastBrushPos)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    BPDrawLine(
                        state, &layer->img, state->lastBrushPos, curPos, dClr
                    );
                    SyncImgLayerObj(state->curLayer);
                }
            }

            if (leftDown) {
                if (!state->brushDragging) {
                    state->brushDragging = true;
                    state->lastBrushPos = curPos;
                }
                BPDrawLine(
                    state, &layer->img, state->lastBrushPos, curPos, dClr
                );
                SyncImgLayerObj(state->curLayer);
                state->lastBrushPos = curPos;
            }

            if (leftReleased && state->brushDragging) {
                state->brushDragging = false;
            }
            break;
        }

        case DT_ERASER: {
            if (!state->eraserDragging && IsKeyDown(KEY_LEFT_SHIFT) &&
                !IsVecNeg(state->lastEraserPos)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    BPDrawLine(
                        state, &layer->img, state->lastEraserPos, curPos, dClr
                    );
                    SyncImgLayerObj(state->curLayer);
                }
            }
            if (leftDown) {
                if (!state->eraserDragging) {
                    state->eraserDragging = true;
                    state->lastEraserPos = curPos;
                }
                BPDrawLine(
                    state, &layer->img, state->lastEraserPos, curPos, dClr
                );
                SyncImgLayerObj(state->curLayer);
                state->lastEraserPos = curPos;
            }
            if (leftReleased && state->eraserDragging) {
                state->eraserDragging = false;
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
                BPDrawLine(state, &layer->img, state->lineStart, curPos, dClr);
                SyncImgLayerObj(state->curLayer);
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
                    state, &layer->img, state->rectStart, curPos, dClr,
                    curtool == DT_RECT_FILL
                );
                SyncImgLayerObj(state->curLayer);
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
                    state, &layer->img, state->circleStart, curPos, dClr,
                    curtool == DT_CIRCLE_FILL, keepRatio
                );
                SyncImgLayerObj(state->curLayer);
                MakeVecZero(&state->circleStart);
                state->circleDragging = false;
            }

            break;
        }
        case DT_BUCKET: {
            if (leftPressed && atCanvas) {
                BpFill(state, &layer->img, curPx, curPy, dClr);
                SyncImgLayerObj(state->curLayer);
            }

            break;
        }

        default:
            break;
        }
    }

    ImageClearBackground(&state->previewImg, BLANK);

    if (atDrawArea && !locked) {

        switch (curtool) {
        case DT_PENCIL: {
            if (!state->brushDragging && IsKeyDown(KEY_LEFT_SHIFT) &&
                !IsVecNeg(state->lastBrushPos)) {
                BPDrawLine(
                    state, &state->previewImg, state->lastBrushPos, curPos, dClr
                );
            }
            DrawBrush(state, &state->previewImg, curPx, curPy, dClr);
            break;
        }
        case DT_ERASER: {
            if (!state->eraserDragging && IsKeyDown(KEY_LEFT_SHIFT) &&
                !IsVecNeg(state->lastEraserPos)) {
                BPDrawLine(
                    state, &state->previewImg, state->lastEraserPos, curPos,
                    EraserColor
                );
            }
            DrawBrush(state, &state->previewImg, curPx, curPy, EraserColor);
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
}

void DrawingCanvasDraw(CanvasState *state, Rectangle bounds) {
    DrawTexture(state->bgTxt, state->drawArea.x, state->drawArea.y, WHITE);
    UpdateTexture(state->previewTxt, state->previewImg.data);
    int curLayerIndex = state->curLayer->index;

    for (int i = state->layers->count - 1; i >= 0; i--) {
        LayerObj *lr = state->layers->layers[i];

        int alpha = (int)(ceilf(lr->opacity * 255));
        Color tint = BpColorSetAlpha(WHITE, alpha);
        if (lr->visible) {
            DrawTexture(lr->txt, state->drawArea.x, state->drawArea.y, tint);
        }

        if (lr->index == curLayerIndex) {
            DrawTexture(
                state->previewTxt, state->drawArea.x, state->drawArea.y, tint
            );
        }
    }
}
