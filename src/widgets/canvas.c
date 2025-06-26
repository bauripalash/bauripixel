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
            c.colors[i][j] = WHITE;
        }
    }

    c.pxSize = INIT_CELL_SIZE;

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

        if (CheckCollisionPointRec(mpos, drawArea) &
            IsKeyDown(KEY_LEFT_SHIFT)) {
            float mouseWheel = GetMouseWheelMove();
            if (mouseWheel != 0) {
                state->zoom *= (mouseWheel > 0 ? 1.1f : 0.9f);
                state->zoom =
                    Clamp(state->zoom, state->zoomMin, state->zoomMax);
            }
        }

        GuiSliderBar(
            (Rectangle){0, 0, 200, 30}, NULL,
            TextFormat("Zoom : %f", state->zoom), &state->zoom, state->zoomMin,
            state->zoomMax
        );

        state->content.width = state->gridSize.x * state->zoom;
        state->content.height = state->gridSize.y * state->zoom;

        BeginScissorMode(
            drawArea.x, drawArea.y, drawArea.width, drawArea.height
        );

        for (int y = 0; y < state->gridSize.y; y++) {
            for (int x = 0; x < state->gridSize.x; x++) {
                Rectangle rect = {
                    drawArea.x + state->scroll.x + x * state->zoom,
                    drawArea.y + state->scroll.y + y * state->zoom,
                    state->zoom,
                    state->zoom,
                };

                if (CheckCollisionPointRec(mpos, rect)) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (ColorIsEqual(state->colors[y][x], WHITE)) {
                            state->colors[y][x] = BLACK;
                        } else {
                            state->colors[y][x] = WHITE;
                        }
                    }
                }

                DrawRectangleRec(rect, state->colors[y][x]);
                DrawRectangleLinesEx(rect, 1.0f, GRAY);
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
