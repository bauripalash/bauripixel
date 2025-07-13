#include "../include/widgets/drawtoolbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/drawtools.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define VISIBLE_BTN_COUNT 4

DrawToolBarState NewDrawToolBar() {
    DrawToolBarState dtb = {0};
    dtb.prop = NewWidgetProp();
    dtb.currentTool = DT_PENCIL;
    dtb.anchor = (Vector2){0, 0};
    dtb.toolbarPadding = (Vector2){0, 10};
    dtb.toolBtnMargin = (Vector2){10, 9};

    dtb.prop.bounds.width = DEFAULT_DTBAR_WIDTH;
    dtb.prop.bounds.height = 0;
    return dtb;
}
void FreeDrawToolBar(DrawToolBarState *state) { return; }

static void updateBounds(DrawToolBarState *dtb) {
    dtb->prop.bounds.x = dtb->anchor.x + DTBAR_MARGIN_L;
    dtb->prop.bounds.y = dtb->anchor.y;

    dtb->prop.bounds.width = DEFAULT_DTBAR_WIDTH;
    dtb->prop.bounds.height = GetScreenHeight() - dtb->prop.bounds.y - 50 - 10;
}

GuiIconName getIconName(int b) {
    if (b == (int)DT_PENCIL) {
        return ICON_PENCIL;
    } else if (b == (int)DT_ERASER) {
        return ICON_RUBBER;
    } else if (b == (int)DT_LINE) {
        return ICON_WAVE_TRIANGULAR;
    } else if (b == (int)DT_PAN) {
        return ICON_CURSOR_HAND;
    }

    return ICON_BOX;
}

#define XBTN_SZ 50

static bool
DtButton(Rectangle bounds, const char *text, GuiIconName icon, bool isActive) {
    float bx = bounds.x;
    float by = bounds.y;
    float bw = bounds.width;
    float bh = bounds.height;
    Color fg = ColorGrayDarkest;
    int iconScale = (int)floorf((float)DEFAULT_DT_BTN_SIZE / 16.0f);
    int iconSize = iconScale * 16;
    bool clicked = CheckCollisionPointRec(GetMousePosition(), bounds) &&
                   IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if (clicked || isActive) {
        by += 3;
        fg = Fade(ColorGrayLighter, 0.5);
    } else {
        DrawRectangleRounded(
            (Rectangle){bx, by, bw, bh + 3}, 0.2, 0, ColorBlack
        );
    }

    Rectangle bRect = {bx, by, bw, bh};
    DrawRectangleRounded(bRect, 0.2, 0, fg);
    DrawRectangleRoundedLinesEx(bRect, 0.2, 0, 2, ColorBlack);
    DrawRectangleRoundedLinesEx(
        (Rectangle){
            bx + 2,
            by + 2,
            bw - 4,
            bh - 4,
        },
        0.2, 0, 2, ColorGrayLightest
    );
    GuiDrawIcon(
        icon, (bx) + (bw / 2.0f) - (iconSize / 2.0f),
        (by) + (bh / 2.0f) - (iconSize / 2.0f), iconScale, ColorWhite
    );

    return clicked;
}

static DrawTool handleShortcuts(DrawToolBarState *state) {
    DrawTool tool = state->currentTool;

    if (IsKeyPressed(KEY_B)) {
        tool = DT_PENCIL;
    } else if (IsKeyPressed(KEY_E)) {
        tool = DT_ERASER;
    } else if (IsKeyPressed(KEY_M)) {
        tool = DT_PAN;
    }

    if (tool != state->currentTool) {
        state->currentTool = tool;
    }

    return tool;
}

static float penSize = 0.0f;
static bool editPenSize = false;
static char penSizeStr[512];
Color clr;

static void OptToolsPencil(DrawToolBarState *state, Rectangle bounds) {
    if (state->currentTool == DT_PENCIL) {
        float px = bounds.x;
        float py = bounds.y;

        Rectangle rect = {
            px + 32, py,

            100, bounds.height
        };

        if (GuiValueBoxFloat(rect, "Size", penSizeStr, &penSize, editPenSize)) {
            editPenSize = !editPenSize;
        }
    }
}

static int DrawOptToolbar(DrawToolBarState *state) {
    Rectangle bounds = {
        state->prop.bounds.x, state->optAnchor.y + DTBAR_MARGIN_TB,
        GetScreenWidth() - DTBAR_MARGIN_R - DTBAR_MARGIN_L, 60 - DTBAR_MARGIN_TB
    };

    Rectangle drawBounds = {
        bounds.x + DTBAR_MARGIN_L, bounds.y + DTBAR_MARGIN_TB,
        bounds.width - DTBAR_MARGIN_L - DTBAR_MARGIN_R,
        bounds.height - DTBAR_MARGIN_TB * 2
    };

    DrawRectangleRounded(bounds, 0.125, 0, ColorFDGrayLighter);

    OptToolsPencil(state, drawBounds);

    DrawRectangleRoundedLinesEx(bounds, 0.125, 0, 3, ColorBlack);
    DrawRectangleRoundedLinesEx(
        (Rectangle){bounds.x + 2, bounds.y + 2, bounds.width - 4,
                    bounds.height - 4},
        0.125, 0, 2, ColorGrayLightest
    );
    return -1;
}

int DrawToolbar(DrawToolBarState *state) {
    updateBounds(state);
    handleShortcuts(state);

    Rectangle bounds = state->prop.bounds;

    DrawOptToolbar(state);

    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    DrawRectangleRounded(
        (Rectangle){state->prop.bounds.x, state->prop.bounds.y,
                    state->prop.bounds.width, state->prop.bounds.height},
        0.125, 0, ColorFDGrayLighter
    );

    for (int btnI = 0; btnI < VISIBLE_BTN_COUNT; btnI++) {
        Rectangle btnRect = {
            (state->prop.bounds.x + state->prop.bounds.width / 2.0f) -
                (DEFAULT_DT_BTN_SIZE / 2.0f),
            state->prop.bounds.y + state->toolbarPadding.y +
                ((DEFAULT_DT_BTN_SIZE + state->toolBtnMargin.y) * btnI),
            DEFAULT_DT_BTN_SIZE,
            DEFAULT_DT_BTN_SIZE,
        };
        if (DtButton(
                btnRect, "", getIconName(btnI), btnI == (int)state->currentTool
            )) {
            if ((int)state->currentTool == btnI) {
                state->currentTool = DT_NOTOOL;
            } else {
                state->currentTool = (DrawTool)btnI;
            }
        }
    }

    DrawRectangleRoundedLinesEx(bounds, 0.125, 0, 3, ColorBlack);
    DrawRectangleRoundedLinesEx(
        (Rectangle){bounds.x + 2, bounds.y + 2, bounds.width - 4,
                    bounds.height - 4},
        0.125, 0, 2, ColorGrayLightest
    );

    return -1;
}
