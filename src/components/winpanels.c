#include "../include/components.h"
#include <stdbool.h>
#include <stdlib.h>

#define CLOSE_BTN_WIDTH 30
#define WIN_MARGIN      10
bool BpSimpleWindow(Rectangle bounds, const char *title) {
    bool result = false;
    BpPanelBorder(bounds, 2);
    BpPanelBorder(
        (Rectangle){bounds.x, bounds.y, bounds.width, WINDOW_TOP_HEIGHT}, 2
    );
    result = GuiLabelButton(
                 (Rectangle){bounds.x + bounds.width - CLOSE_BTN_WIDTH,
                             bounds.y, CLOSE_BTN_WIDTH, WINDOW_TOP_HEIGHT},
                 GuiIconText(ICON_CROSS, NULL)
             ) != 0;

    if (title != NULL) {
        GuiLabel(
            (Rectangle){bounds.x + WIN_MARGIN, bounds.y,
                        bounds.width - CLOSE_BTN_WIDTH, WINDOW_TOP_HEIGHT},
            title
        );
    }

    return result;
}
