#ifndef THEME_H
#define THEME_H

// clang-format off
#include <stdlib.h>
#include <string.h>
#include "../external/raylib.h"
#include "../external/raygui.h"
#include "../include/exported/biosfontii.h"
#include "colors.h"
#include "options.h"
#include "utils.h"
// clang-format on

// ============== OPT THEME START ============
static const OptThemeProp darkThemeOpts[] = {
    {T_SCROLLBAR_BG, HexColorVGray},     {T_SCROLLBAR_FG, HexColorVGreen},
    {T_SCROLLBAR_HVR_BG, HexColorBlack}, {T_SCROLLBAR_HVR_FG, HexColorVGreen},
    {T_SCROLLBAR_CLK_BG, HexColorBlack}, {T_SCROLLBAR_CLK_FG, HexColorVGreen},
    {T_SLIDER_BG, HexColorBlack},        {T_SLIDER_BORDER, HexColorGrayLighter},
    {T_SLIDER_FG, HexColorVGreen},       {T_SLIDER_TEXT_PRE, HexColorVWhite},
    {T_SLIDER_TEXT_POST, HexColorVGray}, {T_ISLIDER_BG, HexColorBlack},
    {T_ISLIDER_FG, HexColorWhite},
};

static const OptThemeProp lightThemeOpts[] = {
    {T_SCROLLBAR_BG, 2},
};
// ============== OPT THEME END ==============

// ============== RAGUI THEME START ============
static const GuiStyleProp RgDarkThemeProps[] = {
    {DEFAULT, TEXT_LINE_SPACING, 1},
    {DEFAULT, TEXT_SIZE, 16},
    {DEFAULT, TEXT_SPACING, 0},

    {VALUEBOX, TEXT_COLOR_NORMAL, HexColorWhite},
    {TEXTBOX, TEXT_COLOR_NORMAL, HexColorWhite},
    {TEXTBOX, TEXT_COLOR_FOCUSED, HexColorWhite},
    {TEXTBOX, TEXT_COLOR_PRESSED, HexColorWhite},
};

// ============== RAGUI THEME END ==============

void LoadAppFont() { GuiLoadStyleBIOSfontII(); }

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

#endif
