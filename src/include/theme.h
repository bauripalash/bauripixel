#ifndef THEME_H
#define THEME_H

#include "../external/raygui.h"
#include "colors.h"
#include "options/options.h"
#include "utils.h"

// ============== OPT THEME START ============
static const OptThemeProp darkThemeOpts[] = {
    {T_SCROLLBAR_BG, HexColorBlack},
    {T_SCROLLBAR_FG, HexColorBlueLighter},
    {T_SCROLLBAR_HVR_BG, HexColorBlack},
    {T_SCROLLBAR_HVR_FG, HexColorBlueLightest},
    {T_SCROLLBAR_CLK_BG, HexColorBlack},
    {T_SCROLLBAR_CLK_FG, HexColorBlueLighter},
    {T_SLIDER_BG, HexColorBlack},
    {T_SLIDER_BORDER, HexColorGrayLighter},
    {T_SLIDER_FG, HexColorGreenLighter},
};

static const OptThemeProp lightThemeOpts[] = {
    {T_SCROLLBAR_BG, 2},
};
// ============== OPT THEME END ==============

// ============== RAGUI THEME START ============
static const GuiStyleProp RgDarkThemeProps[] = {
    {DEFAULT, TEXT_LINE_SPACING, (int)24},

    {VALUEBOX, TEXT_COLOR_NORMAL, HexColorWhite},
    {TEXTBOX, TEXT_COLOR_NORMAL, HexColorWhite},
    {TEXTBOX, TEXT_COLOR_FOCUSED, HexColorWhite},
    {TEXTBOX, TEXT_COLOR_PRESSED, HexColorWhite},
};

// ============== RAGUI THEME END ==============

void LoadAppDarkTheme() {
    for (int i = 0; i < ArrCount(darkThemeOpts); i++) {
        OptThemeSet(darkThemeOpts[i].key, darkThemeOpts[i].value);
    }

    int count = ArrCount(RgDarkThemeProps);
    for (int i = 0; i < count; i++) {
        GuiSetStyle(
            RgDarkThemeProps[i].controlId, RgDarkThemeProps[i].propertyId,
            RgDarkThemeProps[i].propertyValue
        );
    }
}
/*void LoadAppLightTheme() {
    for (int i = 0; i < ArrCount(lightThemeOpts); i++) {
        OptThemeSet(lightThemeOpts[i].key, lightThemeOpts[i].value);
    }
}*/

#endif
