#include "../include/components.h"
#include <stdbool.h>
#include <stdlib.h>

#define CLOSE_BTN_WIDTH  30
#define WIN_MARGIN       10
#define RESIZE_ICON_SIZE 32

bool BpSimpleWindow(
    Rectangle *bounds, const char *title, bool *drag, bool *resize
) {
    bool result = false;
    Vector2 mpos = GetMousePosition();
    Rectangle rdBounds = *bounds;
    BpPanelBorder(rdBounds, 2);
    Rectangle headerRect =
        (Rectangle){rdBounds.x, rdBounds.y, rdBounds.width, WINDOW_TOP_HEIGHT};
    BpPanelBorder(headerRect, 2);

    if (drag != NULL) {

        if (CheckCollisionPointRec(mpos, headerRect) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *drag = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            *drag = false;
        }

        if (*drag) {
            Vector2 delta = GetMouseDelta();
            bounds->x += delta.x;
            bounds->y += delta.y;
        }
    }

    result = GuiLabelButton(
                 (Rectangle){rdBounds.x + rdBounds.width - CLOSE_BTN_WIDTH,
                             rdBounds.y, CLOSE_BTN_WIDTH, WINDOW_TOP_HEIGHT},
                 GuiIconText(ICON_CROSS, NULL)
             ) != 0;

    if (title != NULL) {
        GuiLabel(
            (Rectangle){rdBounds.x + WIN_MARGIN, rdBounds.y,
                        rdBounds.width - CLOSE_BTN_WIDTH, WINDOW_TOP_HEIGHT},
            title
        );
    }

    return result;
}
