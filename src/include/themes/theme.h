#ifndef THEME_H
#define THEME_H

#include "../../external/raygui.h"
#include "../colors.h"
#include "../utils.h"

static const GuiStyleProp LightThemeProps[] = {
    {DEFAULT, TEXT_LINE_SPACING, (int)24},
    //{DEFAULT, BACKGROUND_COLOR, HexColorGrayDarkest},
    //{LABEL, TEXT_COLOR_NORMAL, HexColorBlack},

    {BUTTON, BASE_COLOR_NORMAL, HexColorYellow},
    {VALUEBOX, TEXT_COLOR_NORMAL, HexColorWhite},
    {TEXTBOX, TEXT_COLOR_NORMAL, HexColorWhite},
    {TEXTBOX, TEXT_COLOR_FOCUSED, HexColorWhite},
    {TEXTBOX, TEXT_COLOR_PRESSED, HexColorWhite},
};

void LoadAppFont();
void SetAppIcon();
void LoadAppDarkTheme();
void LoadAppLightTheme() {
    int count = ArrCount(LightThemeProps);
    for (int i = 0; i < count; i++) {
        GuiSetStyle(
            LightThemeProps[i].controlId, LightThemeProps[i].propertyId,
            LightThemeProps[i].propertyValue
        );
    }
}

#endif
