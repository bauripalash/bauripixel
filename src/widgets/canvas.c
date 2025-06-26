#include "../include/widgets/canvas.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

CanvasState NewCanvas() {
    CanvasState c = {0};
    c.prop = NewWidgetProp();
    c.anchor = (Vector2){0, 0};
    c.bottomStop = (Vector2){0, 0};
    c.hoverX = 0;
    c.hoverY = 0;
    c.gridSize = (Vector2){DEFAULT_GRID_SIZE, DEFAULT_GRID_SIZE};

    c.scroll = (Vector2){0, 0};
    c.zoom = 0;
    c.zoomMin = 0;
    c.zoomMax = DEFAULT_GRID_SIZE * 4;
    c.content = (Rectangle){0, 0, 0, 0};
    c.view = (Rectangle){0, 0, 0, 0};
    for (int i = 0; i < (int)c.gridSize.y; i++) {
        for (int j = 0; j < (int)c.gridSize.x; j++) {
            c.colors[i][j] = BLANK;
        }
    }

    c.oldZoom = c.zoom;
    c.oldCw = c.content.width;
    c.oldCh = c.content.height;
    c.oldOffX = 0;
    c.oldOffY = 0;

    return c;
}
void SetCanvasAnchor(CanvasState *c, Vector2 anchor, Vector2 bottom) {
    if (anchor.x != -1) {
        c->anchor.x = anchor.x;
    }
    if (anchor.y != -1) {
        c->anchor.y = anchor.y;
    }
    if (bottom.x != -1) {
        c->bottomStop.x = bottom.x;
    }

    if (bottom.y != -1) {
        c->bottomStop.y = bottom.y;
    }

    c->prop.bounds.x = c->anchor.x + CANVAS_MARGIN_L;
    c->prop.bounds.y = c->anchor.y + CANVAS_MARGIN_TB;
    c->prop.bounds.height = GetScreenHeight() - c->anchor.y - c->bottomStop.y -
                            CANVAS_MARGIN_TB * 2;
    c->prop.bounds.width = GetScreenWidth() - c->anchor.x - CANVAS_MARGIN_L -
                           CANVAS_MARGIN_R - c->bottomStop.x;
}

bool Canvas(CanvasState *state) {
    if (state->prop.active) {
        Vector2 mpos = GetMousePosition();
        Rectangle bounds = state->prop.bounds;

        DrawRectangleRounded(bounds, 0.03, 0, ColorGrayLighter);

        DrawRectangleRoundedLinesEx(
            bounds, 0.03, 0, 5, Fade(ColorGrayLighter, 0.5)
        );

        Rectangle drawArea = {
            bounds.x + CANVAS_DRAW_MARGIN, bounds.y + CANVAS_DRAW_MARGIN,
            bounds.width - (CANVAS_DRAW_MARGIN * 2),
            bounds.height - (CANVAS_DRAW_MARGIN * 2)
        };

        state->zoomMin = fminf(
            drawArea.width / state->gridSize.x,
            drawArea.height / state->gridSize.y
        );
        if (state->zoomMin > state->zoomMax) {
            state->zoomMin = state->zoomMax;
        }

        GuiSliderBar(
            (Rectangle){0, 0, 200, 10}, NULL,
            TextFormat("Zoom : %.2f", state->zoom), &state->zoom,
            state->zoomMin, state->zoomMax
        );

        mpos = GetMousePosition();
        if (CheckCollisionPointRec(mpos, drawArea) &&
            IsKeyDown(KEY_LEFT_SHIFT)) {
            float wheel = GetMouseWheelMove();
            if (wheel != 0.0f) {

                state->zoom *= (wheel > 0 ? 1.1f : 0.9f);
                state->zoom =
                    Clamp(state->zoom, state->zoomMin, state->zoomMax);
            }
        }

        state->content.width = state->gridSize.x * state->zoom;
        state->content.height = state->gridSize.y * state->zoom;

        float offsetX = 0.0f;
        float offsetY = 0.0f;

        if (state->content.width < drawArea.width) {
            offsetX = (drawArea.width - state->content.width) * 0.5f;
        }

        if (state->content.height < drawArea.height) {
            offsetY = (drawArea.height - state->content.height) * 0.5f;
        }

        BeginScissorMode(
            drawArea.x, drawArea.y, drawArea.width, drawArea.height
        );

        for (int row = 0; row < state->gridSize.y; row++) {
            for (int col = 0; col < state->gridSize.x; col++) {
                float px =
                    drawArea.x + state->scroll.x + offsetX + col * state->zoom;
                float py =
                    drawArea.y + state->scroll.y + offsetY + row * state->zoom;

                Rectangle cellRect = {px, py, state->zoom, state->zoom};
                DrawRectangleRec(cellRect, state->colors[row][col]);
                DrawRectangleLinesEx(cellRect, 1, BLACK);

                if (CheckCollisionPointRec(GetMousePosition(), cellRect) &&
                    IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    if (IsKeyDown(KEY_RIGHT_CONTROL) |
                        IsKeyDown(KEY_LEFT_CONTROL)) {
                        state->colors[row][col] = BLANK;
                    } else {
                        state->colors[row][col] = BLACK;
                    }
                }
            }
        }

        EndScissorMode();

        int prev = GuiGetStyle(DEFAULT, BACKGROUND_COLOR);
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0);
        GuiScrollPanel(
            drawArea, NULL, state->content, &state->scroll, &state->view
        );
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, prev);
    }

    return false;
}
