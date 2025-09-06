#include "../external/raygui.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/options.h"
#include <stdbool.h>

bool BpTextBox(Rectangle bounds, char *text, int textSize, bool *edit) {
    bool locked = GuiIsLocked();
    Color inputBg = GetColor(OptThemeGet(T_ISLIDER_BG));
    int textClrInt = OptThemeGet(T_ISLIDER_FG);
    Color inputFg = GetColor(textClrInt);
    Vector2 mpos = GetMousePosition();

    int ogBoxBorderW = GuiGetStyle(VALUEBOX, BORDER_WIDTH);
    int ogBoxBorderN = GuiGetStyle(VALUEBOX, BORDER_COLOR_PRESSED);
    int ogBoxBaseN = GuiGetStyle(VALUEBOX, BASE_COLOR_NORMAL);
    int ogBoxBaseF = GuiGetStyle(VALUEBOX, BASE_COLOR_FOCUSED);
    int ogBoxBaseP = GuiGetStyle(VALUEBOX, BASE_COLOR_PRESSED);
    int ogBoxTextN = GuiGetStyle(VALUEBOX, TEXT_COLOR_NORMAL);
    int ogBoxTextF = GuiGetStyle(VALUEBOX, TEXT_COLOR_FOCUSED);
    int ogBoxTextP = GuiGetStyle(VALUEBOX, TEXT_COLOR_PRESSED);

    GuiSetStyle(VALUEBOX, BORDER_WIDTH, 0);
    GuiSetStyle(VALUEBOX, BORDER_COLOR_PRESSED, textClrInt);

    GuiSetStyle(VALUEBOX, BASE_COLOR_NORMAL, 0);
    GuiSetStyle(VALUEBOX, BASE_COLOR_FOCUSED, 0);
    GuiSetStyle(VALUEBOX, BASE_COLOR_PRESSED, 0);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_NORMAL, textClrInt);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_FOCUSED, textClrInt);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_PRESSED, textClrInt);

    DrawRectangleRounded(bounds, 0.2, 0, Fade(inputBg, 0.8));
    DrawRectangleRoundedLinesEx(bounds, 0.2, 0, 0, ColorTGrayDarker);

    if (GuiTextBox(bounds, text, textSize, *edit)) {
        *edit = !*edit;
    }

    GuiSetStyle(VALUEBOX, BORDER_WIDTH, ogBoxBorderW);
    GuiSetStyle(VALUEBOX, BORDER_COLOR_PRESSED, ogBoxBorderN);
    GuiSetStyle(VALUEBOX, BASE_COLOR_NORMAL, ogBoxBaseN);
    GuiSetStyle(VALUEBOX, BASE_COLOR_FOCUSED, ogBoxBaseF);
    GuiSetStyle(VALUEBOX, BASE_COLOR_PRESSED, ogBoxBaseP);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_NORMAL, ogBoxTextN);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_FOCUSED, ogBoxTextF);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_PRESSED, ogBoxTextP);

    return false;
}

#define DDOWN_PADDING 10
bool BpDropdownBox(
    Rectangle bounds, const char *options, int *selected, bool *clicked
) {
    bool locked = GuiIsLocked();
    Vector2 mpos = GetMousePosition();
    int textClrInt = OptThemeGet(T_ISLIDER_FG);
    Color inputFg = GetColor(textClrInt);

    Color inputBg = GetColor(OptThemeGet(T_ISLIDER_BG));

    DrawRectangleRounded(bounds, 0.2, 0, Fade(inputBg, 0.8));
    DrawRectangleRoundedLinesEx(bounds, 0.2, 0, 0, ColorTGrayDarker);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(mpos, bounds) && !locked) {
        *clicked = !*clicked;
    }

    int numOpt = 0;
    char **optList = TextSplit(options, ';', &numOpt);

    const char *selOpt = optList[*selected];

    Rectangle previewRect = {
        bounds.x + DDOWN_PADDING, bounds.y,
        bounds.width - DDOWN_PADDING * 2 - bounds.height, bounds.height
    };
    GuiLabel(previewRect, selOpt);

    GuiLabel(
        (Rectangle){(bounds.x + bounds.width) - bounds.height, bounds.y,
                    bounds.height, bounds.height},
        GuiIconText(ICON_ARROW_DOWN_FILL, "")
    );

    if (*clicked && numOpt > 1) {
        Rectangle optPanel = {
            bounds.x + DDOWN_PADDING, bounds.y + bounds.height,
            bounds.width - DDOWN_PADDING * 2, bounds.height * (numOpt - 1)
        };

        int ogPanel = OptThemeGetSet(T_PANEL_BG, OptThemeGet(T_ISLIDER_BG));

        BpPanelBorder(optPanel, 2);

        OptThemeSet(T_PANEL_BG, ogPanel);

        float ypos = optPanel.y;
        for (int i = 0; i < numOpt; i++) {
            if (i == *selected) {
                continue;
            }
            Rectangle btnRect = {
                optPanel.x, ypos, optPanel.width, bounds.height
            };
            if (BpLabelButton(btnRect, optList[i])) {
                *selected = i;
                *clicked = false;
            }
            ypos += bounds.height;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            (!CheckCollisionPointRec(mpos, bounds) &&
             !CheckCollisionPointRec(mpos, optPanel)) &&
            !locked) {
            *clicked = false;
        }
    }

    return false;
}
