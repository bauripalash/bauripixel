#include "../include/widgets/drawtoolbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/drawtools.h"

#define VISIBLE_BTN_COUNT 3

DrawToolBarState NewDrawToolBar() {
    DrawToolBarState dtb = {0};
    dtb.prop = NewWidgetProp();
    dtb.currentTool = DT_PENCIL;
    dtb.anchor = (Vector2){0, 0};
    dtb.toolbarPadding = (Vector2){0, 10};
    dtb.toolBtnMargin = (Vector2){10, 5};

    dtb.prop.bounds.width = DEFAULT_DTBAR_WIDTH;
    dtb.prop.bounds.height = 0;
    return dtb;
}
void FreeDrawToolBar(DrawToolBarState *state) { return; }

static void updateBounds(DrawToolBarState *dtb) {
    dtb->prop.bounds.x = dtb->anchor.x;
    dtb->prop.bounds.y = dtb->anchor.y;

    dtb->prop.bounds.width = DEFAULT_DTBAR_WIDTH;
    dtb->prop.bounds.height = GetScreenHeight() - dtb->prop.bounds.y - 50 - 10;
}

GuiIconName getIconName(int b) {
    if (b == (int)DT_PENCIL) {
        return ICON_PENCIL_BIG;
    } else if (b == (int)DT_ERASER) {
        return ICON_RUBBER;
    } else if (b == (int)DT_LINE) {
        return ICON_WAVE_TRIANGULAR;
    }

    return ICON_BOX;
}

int DrawToolbar(DrawToolBarState *state) {
    updateBounds(state);
    DrawRectangleRounded(
        (Rectangle){state->prop.bounds.x, state->prop.bounds.y,
                    state->prop.bounds.width, state->prop.bounds.height},
        0.125, 0, ColorGrayLighter
    );

    DrawRectangleRoundedLinesEx(
        state->prop.bounds, 0.125, 0, 2, ColorGrayLightest
    );

    for (int btnI = 0; btnI < VISIBLE_BTN_COUNT; btnI++) {
        Rectangle btnRect = {
            (state->prop.bounds.width / 2.0f) - (DEFAULT_DT_BTN_SIZE / 2.0f),
            state->prop.bounds.y + state->toolbarPadding.y +
                ((DEFAULT_DT_BTN_SIZE + state->toolBtnMargin.y) * btnI),
            DEFAULT_DT_BTN_SIZE,
            DEFAULT_DT_BTN_SIZE,
        };

        DrawRectangleRec(btnRect, ColorGrayLightest);
        DrawRectangleLinesEx(btnRect, 1, ColorBlack);
        btnRect.x += 16;
        GuiLabelButton(btnRect, GuiIconText(getIconName(btnI), ""));

        if (CheckCollisionPointRec(GetMousePosition(), btnRect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->currentTool = (DrawTool)btnI;
        }
    }

    return -1;
}
