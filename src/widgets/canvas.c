#include "../include/widgets/canvas.h"
#include "../external/raygui.h"
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

        DrawRectangleRec(drawArea, Fade(ColorWhite, 0.1));

        float smallestLimit = fminf(drawArea.width, drawArea.height);
        float smallestCell = fminf(state->gridSize.x, state->gridSize.y);
        TraceLog(LOG_WARNING, "W[%f] | H[%f]", drawArea.width, drawArea.height);

        float cellSize = smallestLimit / 16.0f;

        for (int row = 0; row < state->gridSize.y; row++) {
            for (int col = 0; col < state->gridSize.x; col++) {
                Rectangle cellRect = {
                    drawArea.x + (col * cellSize),
                    drawArea.y + (row * cellSize), cellSize, cellSize
                };
                DrawRectangleRec(cellRect, ColorWhite);
                DrawRectangleLinesEx(cellRect, 1, Fade(ColorGrayDarkest, 0.1));
            }
        }

        // GuiGrid(drawArea, NULL, drawArea.height / 16.0f, 1, NULL);
        // GuiGrid(drawArea, NULL, cellSize, 1, NULL);
    }

    return false;
}
