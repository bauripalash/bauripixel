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
