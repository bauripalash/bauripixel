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

#define xclr BpHexColor(333f58)
#define sclr BpHexColor(292831)

// ============== OPT THEME START ============
static const OptThemeProp darkThemeOpts[] = {
    {T_SCROLLBAR_BG, HexColorVGray},
    {T_SCROLLBAR_FG, HexColorVGreen},
    {T_SCROLLBAR_HVR_BG, HexColorBlack},
    {T_SCROLLBAR_HVR_FG, HexColorVGreen},
    {T_SCROLLBAR_CLK_BG, HexColorBlack},
    {T_SCROLLBAR_CLK_FG, HexColorVGreen},
    {T_SLIDER_BG, HexColorBlack},
    {T_SLIDER_BORDER, HexColorGrayLighter},
    {T_SLIDER_FG, HexColorVGreen},
    {T_SLIDER_TEXT_PRE, HexColorVWhite},
    {T_SLIDER_TEXT_POST, HexColorVGray},
    {T_ISLIDER_BG, HexColorBlack},
    {T_ISLIDER_FG, HexColorWhite},
    {T_PANEL_BG, HexColorCPanel},
    {T_PANEL_SHADOW, HexColorBlack},
    {T_PANEL_BORDER, HexColorSWhite},
    {T_APP_BG, HexColorCPanel},
    {T_TAB_PANEL_BG, HexColorGrayLighter},
    {T_TAB_ITEM_ACTIVE, HexColorGrayLighter},
    {T_TAB_ITEM_INACTIVE, HexColorCPanel},

    {T_BTN_BG, HexColorGrayDarker},
    {T_BTN_HVR_BG, HexColorGrayLighter},
    {T_BTN_CLK_BG, HexColorGrayLighter},
    {T_BTN_BORDER, HexColorWhite},
    {T_BTN_CORNER, HexColorGrayLighter},
    {T_BTN_SHADOW, HexColorBlack},
    {T_BTN_BRDR_THICK, 2},
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
    {LABEL, TEXT_COLOR_NORMAL, HexColorWhite},
    {LABEL, TEXT_COLOR_FOCUSED, HexColorWhite},
    {LABEL, TEXT_COLOR_PRESSED, HexColorWhite},
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
