#ifndef RGTHEMES_H
#define RGTHEMES_H

// clang-format off
#include "colors.h"
#include "../external/raylib.h"
#include "../external/raygui.h"
#include "utils.h"
// clang-format on

static const GuiStyleProp DarkThemeProps[] = {
    {BUTTON, BASE_COLOR_NORMAL, HexColorGrayLighter},
};

void LoadAppDarkTheme() {
    for (int i = 0; i < ArrCount(DarkThemeProps); i++) {
        GuiSetStyle(
            DarkThemeProps[i].controlId, DarkThemeProps[i].propertyId,
            DarkThemeProps[i].propertyValue
        );
    }
}

#endif
