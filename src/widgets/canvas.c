#include "../include/widgets/canvas.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include "raymath.h"
#include <stdbool.h>
#include <stddef.h>

CanvasState NewCanvas() {
    CanvasState c = {0};
    c.prop = NewWidgetProp();
    c.hoverX = 0;
    c.hoverY = 0;
    c.gridSize = (Vector2){DEFAULT_GRID_SIZE, DEFAULT_GRID_SIZE};

    c.scroll = (Vector2){0, 0};
    c.zoomMin = 0.2f;
    c.zoomMax = INIT_CELL_SIZE;

    for (int i = 0; i < (int)c.gridSize.y; i++) {
        for (int j = 0; j < (int)c.gridSize.x; j++) {
            c.colors[i][j] = ColorWhite;
        }
    }

    c.colors[0][0] = ColorBlack;

    c.pxSize = INIT_CELL_SIZE;
    c.prop.bounds.x = CANVAS_MARGIN_L;
    c.prop.bounds.y = CANVAS_MARGIN_TB;
    c.prop.bounds.width =
        GetScreenWidth() - (CANVAS_MARGIN_L + CANVAS_MARGIN_R);
    c.prop.bounds.height = GetScreenHeight() - (CANVAS_MARGIN_TB * 2);

    c.camera = (Camera2D){0};
    c.camera.zoom = 1.0f;

    return c;
}

void updateBounds(CanvasState *c) {
    c->prop.bounds.x = CANVAS_MARGIN_L;
    c->prop.bounds.y = CANVAS_MARGIN_TB;
    c->prop.bounds.width =
        GetScreenWidth() - (CANVAS_MARGIN_L + CANVAS_MARGIN_R);
    c->prop.bounds.height = GetScreenHeight() - (CANVAS_MARGIN_TB * 2);
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

bool sliderDrag = false;
int minVal = 0;
int maxVal = 50;
int BauScrollBar(Rectangle rect, int *displ) {
    int value = 0;
    int borderWidth = 2;
    Rectangle sliderRect = {
        rect.x + *displ,
        rect.y,
        100,
        rect.height,
    };

    DrawRectangleRec(rect, Fade(ColorWhite, 0.9));

    float minSliderX = rect.x;
    float maxSliderX = rect.x + rect.width - sliderRect.width;

    if (CheckCollisionPointRec(GetMousePosition(), sliderRect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        sliderDrag = true;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        sliderDrag = false;
    }

    if (sliderDrag) {
        Vector2 delta = GetMouseDelta();

        sliderRect.x += delta.x;
        sliderRect.x = Clamp(sliderRect.x, minSliderX, maxSliderX);
        *displ = sliderRect.x - rect.x;
    }

    DrawRectangleRec(sliderRect, ColorGrayDarkest);

    DrawRectangleLinesEx(rect, borderWidth, ColorBlack);

    return value;
}

float value = 0.0f;
int val = 0;
float offsetX;
bool Canvas(CanvasState *state) {
    if (state->prop.active) {
        updateBounds(state);
        Vector2 mpos = GetMousePosition();
        Rectangle bounds = state->prop.bounds;

        DrawRectangleRounded(bounds, 0.03, 0, ColorGrayLighter);

        DrawRectangleRoundedLinesEx(bounds, 0.03, 0, 5, Fade(ColorWhite, 0.5));

        Rectangle drawArea = {
            bounds.x + CANVAS_DRAW_MARGIN, bounds.y + CANVAS_DRAW_MARGIN,
            bounds.width - (CANVAS_DRAW_MARGIN * 2),
            bounds.height - (CANVAS_DRAW_MARGIN * 2)
        };

        Rectangle canvas = {
            drawArea.x,
            drawArea.y,
            drawArea.x + state->gridSize.x * state->pxSize,
            drawArea.y + state->gridSize.y * state->pxSize,
        };

        DrawRectangleRec(drawArea, ColorWhite);

        if (CheckCollisionPointRec(mpos, drawArea)) {
            float wheel = GetMouseWheelMove();

            if (wheel != 0) {
                Vector2 mouseWorldPos =
                    GetScreenToWorld2D(GetMousePosition(), state->camera);
                state->camera.offset = GetMousePosition();
                state->camera.target = mouseWorldPos;
                float scale = 0.2f * wheel;
                state->camera.zoom = expf(logf(state->camera.zoom) + scale);
                state->camera.zoom =
                    Clamp(state->camera.zoom, state->zoomMin, state->zoomMax);
                offsetX = state->camera.offset.x;

                TraceLog(LOG_WARNING, "Zoom : %f", state->camera.zoom);
            }

            if (IsKeyDown(KEY_LEFT_SHIFT) &&
                IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 delta = GetMouseDelta();
                delta = Vector2Scale(delta, -1.0 / state->camera.zoom);
                state->camera.target = Vector2Add(state->camera.target, delta);
            }
        }

        // int canvasSizeW = (int)(state->gridSize.x * state->pxSize *
        // state->camera.zoom); if (state->camera.target.x > canvasSizeW) {
        //	state->camera.target.x = canvasSizeW;
        // }

        BeginScissorMode(
            drawArea.x, drawArea.y, drawArea.width, drawArea.height
        );
        BeginMode2D(state->camera);
        {
            for (int row = 0; row < (int)state->gridSize.y; row++) {
                for (int col = 0; col < (int)state->gridSize.x; col++) {
                    Rectangle rect = {
                        drawArea.x + (col * state->pxSize),
                        drawArea.y + (row * state->pxSize),
                        state->pxSize,
                        state->pxSize,
                    };

                    Vector2 mouseWorldPos =
                        GetScreenToWorld2D(GetMousePosition(), state->camera);
                    if (CheckCollisionPointRec(mouseWorldPos, rect) &&
                        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (ColorIsEqual(state->colors[row][col], ColorWhite)) {
                            state->colors[row][col] = ColorBlack;
                        } else {
                            state->colors[row][col] = ColorWhite;
                        }
                        TraceLog(LOG_WARNING, "CLICKED->[%d,%d]", col, row);
                    }

                    DrawRectangleRec(rect, state->colors[row][col]);
                    DrawRectangleLinesEx(rect, 1, Fade(ColorGrayDarkest, 0.5));
                }
            }

            DrawLineEx(
                (Vector2){drawArea.x, drawArea.y}, state->camera.target, 2,
                ColorGreenLighter
            );
        }

        EndMode2D();
        EndScissorMode();

        Vector2 worldTL = {canvas.x, canvas.y};
        Vector2 worldBR = {canvas.width, canvas.height};

        Vector2 screenTL = GetWorldToScreen2D(worldTL, state->camera);
        Vector2 screenBR = GetWorldToScreen2D(worldBR, state->camera);

        Vector2 areaWorldTL = GetScreenToWorld2D(
            (Vector2){drawArea.x, drawArea.y}, state->camera
        );
        Vector2 areaWorldBR = GetScreenToWorld2D(
            (Vector2){drawArea.width + drawArea.x,
                      drawArea.height + drawArea.y},
            state->camera
        );

        TraceVector(state->camera.target, "Target ->");
        TraceVector(areaWorldTL, "World TL");

        TraceVector(areaWorldBR, "BR ->");

        TraceRect(canvas, "Canvas -> ");
        if (areaWorldTL.x > canvas.width) {
            TraceLog(LOG_ERROR, "Outside [Left]");
            state->camera.target.x -= areaWorldTL.x - canvas.x - 512;
        }

        if (areaWorldTL.y > canvas.height) {
            TraceLog(LOG_ERROR, "Outside [Top]");
            TraceVector(state->camera.target, "Target ->");
            state->camera.target.y -= areaWorldTL.y - canvas.y - 512;
        }

        if (areaWorldBR.x < canvas.x) {
            TraceLog(LOG_ERROR, "Outside [Right]");

            TraceVector(areaWorldBR, "BR ->");
            TraceVector(state->camera.target, "Target ->");
        }

        if (areaWorldBR.y < canvas.y) {
            TraceLog(LOG_ERROR, "Outside [Bottom]");

            TraceVector(state->camera.target, "Target ->");
        }

        float contentWidth = 500.0f;
        float viewWidth = 300.0f;

        Rectangle sliderRect = {
            drawArea.x, drawArea.y + drawArea.height, drawArea.width, 20
        };
        float maxOffset =
            (state->gridSize.x * state->pxSize * state->camera.zoom);

        /*
        TraceLog(LOG_WARNING, "CAMERA -> TARGET[%f,%f] | OFFSET[%f,%f] |
        ZOOM[%f] | ROT[%f]" , state->camera.target.x, state->camera.target.y,
           state->camera.offset.x,
           state->camera.offset.y,
           state->camera.zoom,
           state->camera.rotation
        );
        */
    }

    return false;
}
