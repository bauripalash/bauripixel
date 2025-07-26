#include "../include/widgets/colorbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../external/stb/stb_ds.h"
#include "../include/colors.h"
#include "../include/options.h"
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

    cb.prop.bounds.x = cb.anchor.x + CBAR_MARGIN_LEFT - CBAR_MARGIN_RIGHT;
    cb.prop.bounds.y = cb.anchor.y + CBAR_MARGIN_TOPBOTTOM;
    cb.prop.bounds.width = CBAR_INIT_WIDTH;
    cb.prop.bounds.height = CBAR_INIT_HEIGHT;

    cb.boxSize = CBAR_BOX_SIZE;

    cb.widthDragging = false;
    cb.heightDragging = false;

    cb.hoverColorList = false;
    cb.hoverIndex = -1;
    cb.hoverColor = BLACK;

    return cb;
}

static void updateBounds(ColorBarState *state) {
    state->prop.bounds.x =
        GetScreenWidth() - state->prop.bounds.width - CBAR_MARGIN_RIGHT;
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

bool CurrentColorChanged(ColorBarState *state) {
    return state->prevIndex != state->currentIndex;
}

Rectangle view = {0};
#define SELECT_THICKNESSS 3.0f

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

        if (atBounds) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

            if (IsKeyDown(KEY_LEFT_CONTROL)) {
                float wheel = GetMouseWheelMove();
                if (wheel != 0) {
                    float scale = 0.2f * wheel;
                    state->boxSize = Clamp(
                        expf(logf(state->boxSize) + scale),
                        CBAR_BOX_SIZE / 2.0f, CBAR_BOX_SIZE * 2
                    );
                }
            }
        }

        if (atHandle && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->widthDragging = true;
        }

        if (state->widthDragging) {
            Vector2 delta = GetMouseDelta();
            state->prop.bounds.width -= delta.x;
        }

        state->prop.bounds.width = Clamp(
            state->prop.bounds.width, CBAR_MIN_WIDTH, CBAR_INIT_WIDTH * 2.0f
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

        state->hoverColorList = CheckCollisionPointRec(mpos, usableRect);

        state->prevIndex = state->currentIndex;
        BeginScissorMode(
            usableRect.x, usableRect.y, usableRect.width, usableRect.height
        );
        {
            for (int b = 0; b < colorCount; b++) {
                int col = b % maxColumns;
                int row = b / maxColumns;
                Color clr = state->colors[b];
                Color borderClr = ColorBlack;

                Rectangle boxRect = {
                    usableRect.x + state->scroll.x + (col * boxSize),
                    usableRect.y + state->scroll.y + (row * boxSize), boxSize,
                    boxSize
                };

                DrawRectangleRec(boxRect, clr);

                if (b == state->currentIndex) {
                    borderClr = ColorWhite;
                }

                DrawRectangleLinesEx(boxRect, 2, borderClr);

                if (CheckCollisionPointRec(mpos, boxRect)) {
                    state->hoverColor = clr;
                    state->hoverIndex = b;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        state->currentIndex = b;
                        state->currentColor = clr;
                    }
                }
            }
        }
        EndScissorMode();

        int ogDefBG = GuiGetStyle(DEFAULT, BACKGROUND_COLOR);
        int ogBorderWidth = GuiGetStyle(LISTVIEW, BORDER_WIDTH);
        int ogBtnBase = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
        int ogSliderBorderN = GuiGetStyle(SLIDER, BORDER_COLOR_NORMAL);
        int ogSliderBorderF = GuiGetStyle(SLIDER, BORDER_COLOR_FOCUSED);
        int ogSliderBorderP = GuiGetStyle(SLIDER, BORDER_COLOR_PRESSED);

        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, HexColorTransparent);
        GuiSetStyle(LISTVIEW, BORDER_WIDTH, 0);

        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, OptThemeGet(T_SCROLLBAR_BG));
        GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, OptThemeGet(T_SCROLLBAR_FG));
        GuiSetStyle(
            SLIDER, BORDER_COLOR_FOCUSED, OptThemeGet(T_SCROLLBAR_HVR_FG)
        );
        GuiSetStyle(
            SLIDER, BORDER_COLOR_PRESSED, OptThemeGet(T_SCROLLBAR_CLK_FG)
        );

        GuiScrollPanel(usableRect, NULL, usedRect, &state->scroll, &view);

        GuiSetStyle(LISTVIEW, BORDER_WIDTH, ogBorderWidth);
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ogDefBG);
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ogBtnBase);
        GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, ogSliderBorderN);
        GuiSetStyle(SLIDER, BORDER_COLOR_FOCUSED, ogSliderBorderF);
        GuiSetStyle(SLIDER, BORDER_COLOR_PRESSED, ogSliderBorderP);

        DrawRectangleRoundedLinesEx(bounds, 0.125, 0, 3, ColorBlack);
        DrawRectangleRoundedLinesEx(
            (Rectangle){bounds.x + 2, bounds.y + 2, bounds.width - 4,
                        bounds.height - 4},
            0.125, 0, 2, ColorGrayLightest
        );
    }

    return CB_STATUS_NONE;
}
