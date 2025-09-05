#include "../include/widgets/colorbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../external/stb/stb_ds.h"
#include "../include/colors.h"
#include "../include/components.h"
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
    cb.bottom = (Vector2){0, 0};
    cb.scroll = (Vector2){0, 0};
    cb.view = (Rectangle){0};
    cb.usableRect = (Rectangle){0};
    cb.usedRect = (Rectangle){0};
    cb.maxColumns = 0;
    cb.usedColumns = 0;
    cb.usedRows = 0;

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
        state->bottom.x - state->prop.bounds.width - CBAR_MARGIN_RIGHT;

    state->prop.bounds.y = state->anchor.y + CBAR_MARGIN_TOPBOTTOM;
}
void SetColorBarAnchor(ColorBarState *state, Vector2 anchor, Vector2 bottom) {

    if (anchor.x >= 0) {
        state->anchor.x = anchor.x;
    }

    if (anchor.y >= 0) {
        state->anchor.y = anchor.y;
    }

    if (bottom.x != -1) {
        state->bottom.x = bottom.x;
    }

    if (bottom.y != -1) {
        state->bottom.y = bottom.y;
    }

    updateBounds(state);
}

bool CurrentColorChanged(ColorBarState *state) {
    return state->prevIndex != state->currentIndex;
}

Rectangle view = {0};
#define SELECT_THICKNESSS 3.0f

int ColorBarLogic(ColorBarState *state) {
    if (state->prop.active) {
        updateBounds(state);
        bool locked = GuiIsLocked();

        Rectangle bounds = state->prop.bounds;

        Rectangle handleRect = {
            bounds.x - (CBAR_HANDLE_THICKNESS / 2.0f), bounds.y,
            CBAR_HANDLE_THICKNESS, bounds.height
        };
        Vector2 mpos = GetMousePosition();
        bool atBounds = CheckCollisionPointRec(mpos, bounds);
        bool atHandle = CheckCollisionPointRec(mpos, handleRect);

        if (atBounds) {
            //SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            if (IsKeyDown(KEY_LEFT_CONTROL) && !locked) {
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

        if (atHandle && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !locked) {
            state->widthDragging = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !locked) {
            state->widthDragging = false;
        }

        if (state->widthDragging) {
            Vector2 delta = GetMouseDelta();
            state->prop.bounds.width -= delta.x;
        }

        state->prop.bounds.width = Clamp(
            state->prop.bounds.width, CBAR_MIN_WIDTH, CBAR_INIT_WIDTH * 2.0f
        );

        bounds = state->prop.bounds;

        float colorBoxSize = state->boxSize;
        float halfBox = colorBoxSize / 2.0f;
        int colorCount = state->colorCount;

        Rectangle usableRect = {
            bounds.x + halfBox, bounds.y + halfBox, bounds.width - colorBoxSize,
            bounds.height - colorBoxSize
        };

        int maxColumns = (int)floorf(usableRect.width / colorBoxSize);
        int usedRows = colorCount / maxColumns;
        int usedColumns = (colorCount < maxColumns) ? colorCount : maxColumns;

        if ((colorCount % maxColumns) > 0) {
            usedRows++;
        }

        Rectangle usedRect = {
            usableRect.x, usableRect.y, usedColumns * colorBoxSize,
            usedRows * colorBoxSize
        };

        state->hoverColorList = CheckCollisionPointRec(mpos, usableRect);

        state->prevIndex = state->currentIndex;
        for (int c = 0; c < colorCount; c++) {
            int col = c % maxColumns;
            int row = c / maxColumns;
            Rectangle colorRect = {
                usableRect.x + state->scroll.x + (col * colorBoxSize),
                usableRect.y + state->scroll.y + (row * colorBoxSize),
                colorBoxSize,
                colorBoxSize,
            };

            if (CheckCollisionRecs(state->view, colorRect) &&
                CheckCollisionPointRec(GetMousePosition(), colorRect)) {
                state->hoverColor = state->colors[c];
                state->hoverIndex = c;

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !locked) {
                    state->currentIndex = c;
                    state->currentColor = state->hoverColor;
                }
            }
        }

        state->usableRect = usableRect;
        state->maxColumns = maxColumns;
        state->usedColumns = usedColumns;
        state->usedRows = usedRows;
        state->usedRect = usedRect;
    }
    return -1;
}

int ColorBarDraw(ColorBarState *state) {
    if (state->prop.active) {
        Rectangle bounds = state->prop.bounds;
        float colorBoxSize = state->boxSize;
        float halfBoxSize = colorBoxSize / 2.0f;
        int colorCount = state->colorCount;
        // BpRoundedPanel(bounds, 2, 0.125, true);
        BpPanelBorder(bounds, 2);

        Rectangle usableRect = state->usableRect;

        int maxColumns = state->maxColumns;
        int usedColumns = state->usedColumns;
        int usedRows = state->usedRows;

        BeginScissorMode(
            usableRect.x, usableRect.y, usableRect.width, usableRect.height
        );

        for (int c = 0; c < colorCount; c++) {
            int col = c % maxColumns;
            int row = c / maxColumns;

            Color clr = state->colors[c];

            Color borderClr =
                state->currentIndex == c ? ColorWhite : ColorBlack;

            Rectangle colorRect = {
                usableRect.x + state->scroll.x + col * colorBoxSize,
                usableRect.y + state->scroll.y + row * colorBoxSize,
                colorBoxSize, colorBoxSize
            };

            DrawRectangleRec(colorRect, clr);
            DrawRectangleLinesEx(colorRect, 2, borderClr);
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

        GuiScrollPanel(
            state->usableRect, NULL, state->usedRect, &state->scroll,
            &state->view
        );

        GuiSetStyle(LISTVIEW, BORDER_WIDTH, ogBorderWidth);
        GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ogDefBG);
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ogBtnBase);
        GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, ogSliderBorderN);
        GuiSetStyle(SLIDER, BORDER_COLOR_FOCUSED, ogSliderBorderF);
        GuiSetStyle(SLIDER, BORDER_COLOR_PRESSED, ogSliderBorderP);
    }

    return -1;
}
