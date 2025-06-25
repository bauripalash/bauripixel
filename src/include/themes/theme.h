#ifndef THEME_H
#define THEME_H

#include "../../external/raygui.h"
#include "../colors.h"

static const GuiStyleProp LightThemeProps[] = {
    {DEFAULT, TEXT_LINE_SPACING, (int)24},
    //{DEFAULT, BACKGROUND_COLOR, HexColorGrayDarkest},
    {LABEL, TEXT_COLOR_NORMAL, HexColorBlack},
};

#define PropCount(a) (sizeof(a) / sizeof((a)[0]))

void LoadAppFont();
void SetAppIcon();
void LoadAppDarkTheme();
void LoadAppLightTheme() {
    int count = PropCount(LightThemeProps);
    for (int i = 0; i < count; i++) {
        GuiSetStyle(
            LightThemeProps[i].controlId, LightThemeProps[i].propertyId,
            LightThemeProps[i].propertyValue
        );
    }
}

#endif
