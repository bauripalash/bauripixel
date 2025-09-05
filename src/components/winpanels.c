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

    if (resize != NULL) {
        Rectangle handle = {
            rdBounds.x + rdBounds.width - RESIZE_ICON_SIZE,
            rdBounds.y + rdBounds.height - RESIZE_ICON_SIZE, RESIZE_ICON_SIZE,
            RESIZE_ICON_SIZE
        };
        GuiDrawIcon(ICON_CORNER, handle.x, handle.y, 2, WHITE);

        if (CheckCollisionPointRec(mpos, handle) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *resize = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            *resize = false;
        }

        if (*resize) {
            Vector2 delta = GetMouseDelta();
            bounds->width += delta.x;
            bounds->height += delta.y;
        }
    }

    return result;
}
