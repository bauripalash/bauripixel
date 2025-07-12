#include "../include/widgets/colorbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../external/stb/stb_ds.h"
#include "../include/colors.h"
#include <math.h>
#include <stdbool.h>

ColorBarState NewColorBar() {
    ColorBarState cb = {0};
    cb.prop = NewWidgetProp();

    cb.currentIndex = -1;
    cb.currentColor = BLACK;
    cb.colors = NULL;
    cb.colorCount = 0;

    cb.anchor = (Vector2){0, 0};
    cb.bottomStop = (Vector2){0, 0};
    cb.scroll = (Vector2){0, 0};

    cb.prop.bounds.x = cb.anchor.x + CBAR_MARGIN_LEFT;
    cb.prop.bounds.y = cb.anchor.y + CBAR_MARGIN_TOPBOTTOM;
    cb.prop.bounds.width = CBAR_INIT_WIDTH;
    cb.prop.bounds.height = CBAR_INIT_HEIGHT;

    cb.boxSize = CBAR_BOX_SIZE;

    cb.widthDragging = false;
    cb.heightDragging = false;

    return cb;
}

static void updateBounds(ColorBarState *state) {
    state->prop.bounds.x = GetScreenWidth() - state->prop.bounds.width;
    state->prop.bounds.y = state->anchor.y + CBAR_MARGIN_TOPBOTTOM;
}
void SetColorBarAnchor(ColorBarState *state, Vector2 anchor, Vector2 bottom) {

    if (anchor.x >= 0) {
        state->anchor.x = anchor.x;
    }

    if (anchor.y >= 0) {
        state->anchor.y = anchor.y;
    }

    updateBounds(state);
}

int AddToColorBar(ColorBarState *cb, Color color) {
    arrpush(cb->colors, color);
    cb->colorCount = arrlen(cb->colors);

    return cb->colorCount - 1;
}

void ClearColorBar(ColorBarState *cb) {
    if (cb->colors != NULL) {
        arrfree(cb->colors);
    }
}

int ColorBar(ColorBarState *state) {
    if (state->prop.active) {
        updateBounds(state);

        Rectangle bounds = state->prop.bounds;
        Vector2 mpos = GetMousePosition();

        Rectangle handleRect = {
            bounds.x - (CBAR_HANDLE_THICKNESS / 2.0f), bounds.y,
            CBAR_HANDLE_THICKNESS, bounds.height
        };

        bool atBounds = CheckCollisionPointRec(mpos, bounds);
        bool atHandle = CheckCollisionPointRec(mpos, handleRect);

        if (atHandle && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->widthDragging = true;
        }

        if (state->widthDragging) {
            Vector2 delta = GetMouseDelta();
            state->prop.bounds.width -= delta.x;
        }

        state->prop.bounds.width = Clamp(
            state->prop.bounds.width, CBAR_INIT_WIDTH, CBAR_INIT_WIDTH * 2.0f
        );

        bounds = state->prop.bounds;

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            state->widthDragging = false;
        }

        DrawRectangleRounded(bounds, 0.125f, 0, ColorFDGrayLighter);

        float boxSize = state->boxSize;
        float halfBox = boxSize / 2.0f;
        int colorCount = state->colorCount;

        Rectangle usableRect = {
            bounds.x + halfBox, bounds.y + halfBox, bounds.width - boxSize,
            bounds.height - boxSize
        };

        int maxColumns = (int)floorf(usableRect.width / boxSize);
        int maxRows = (int)floorf(usableRect.height / boxSize);

        int usedRows = colorCount / maxColumns;
        int usedColumns = (colorCount < maxColumns) ? colorCount : maxColumns;
        // how many boxes are in last row
        int lastRow = colorCount % maxColumns;

        if (lastRow > 0) {
            usedRows++;
        }

        Rectangle usedRect = {
            usableRect.x,
            usableRect.y,
            usedColumns * boxSize,
            usedRows * boxSize,
        };

        BeginScissorMode(
            usableRect.x, usableRect.y, usableRect.width, usableRect.height
        );
        {
            for (int b = 0; b < colorCount; b++) {
                int col = b % maxColumns;
                int row = b / maxColumns;
                Color clr = state->colors[b];

                Rectangle boxRect = {
                    usableRect.x + state->scroll.x + (col * boxSize),
                    usableRect.y + state->scroll.y + (row * boxSize), boxSize,
                    boxSize
                };

                DrawRectangleRec(boxRect, clr);

                if (ColorIsEqual(state->currentColor, clr)) {
                    DrawRectangleLinesEx(boxRect, 1.5, BLACK);
                }

                if (CheckCollisionPointRec(mpos, boxRect) &&
                    IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    state->currentColor = clr;
                }
            }
        }
        EndScissorMode();

        int ogDefBG = GuiGetStyle(DEFAULT, BACKGROUND_COLOR);
        int ogBorderWidth = GuiGetStyle(LISTVIEW, BORDER_WIDTH);
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, HexColorTransparent);
        GuiSetStyle(LISTVIEW, BORDER_WIDTH, 0);

        GuiScrollPanel(usableRect, NULL, usedRect, &state->scroll, NULL);

        GuiSetStyle(LISTVIEW, BORDER_WIDTH, ogBorderWidth);
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ogDefBG);

        DrawRectangleRoundedLinesEx(bounds, 0.125, 0, 3, ColorBlack);
        DrawRectangleRoundedLinesEx(
            (Rectangle){bounds.x + 2, bounds.y + 2, bounds.width - 4,
                        bounds.height - 4},
            0.125, 0, 2, ColorGrayLightest
        );
    }
    return CB_STATUS_NONE;
}
