#include "../include/components.h"
#include <stdbool.h>
#include <stdlib.h>

#define TITLE_HEIGHT    30
#define CLOSE_BTN_WIDTH 30
#define WIN_MARGIN      10
bool BpSimpleWindow(Rectangle bounds, const char *title) {
    bool result = false;
    BpPanelBorder(bounds, 2);
    BpPanelBorder(
        (Rectangle){bounds.x, bounds.y, bounds.width, TITLE_HEIGHT}, 2
    );
    result = GuiLabelButton(
                 (Rectangle){bounds.x + bounds.width - CLOSE_BTN_WIDTH,
                             bounds.y, CLOSE_BTN_WIDTH, TITLE_HEIGHT},
                 GuiIconText(ICON_CROSS, NULL)
             ) != 0;

    if (title != NULL) {
        GuiLabel(
            (Rectangle){bounds.x + WIN_MARGIN, bounds.y,
                        bounds.width - CLOSE_BTN_WIDTH, TITLE_HEIGHT},
            title
        );
    }

    return result;
}
