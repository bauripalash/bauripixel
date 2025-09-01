#ifndef THEME_H
#define THEME_H

// clang-format off
#include <stdlib.h>
#include <string.h>
#include "../external/raylib.h"
#include "../external/raygui.h"
#include "../include/exported/biosfontii.h"
#include "../include/exported/appicon.h"
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
    {T_SLIDER_BORDER, HexColorVGrayLight},
    {T_SLIDER_FG, HexColorVGreen},
    {T_SLIDER_TEXT_PRE, HexColorVWhite},
    {T_SLIDER_TEXT_POST, HexColorVGray},
    {T_ISLIDER_BG, HexColorBlack},
    {T_ISLIDER_FG, HexColorWhite},
    {T_PANEL_BG, HexColorStatus},

    {T_PANEL_SHADOW, HexColorBlack},
    {T_PANEL_BORDER, HexColorBlack},
    {T_APP_BG, HexColorAppBG},
    {T_TAB_PANEL_BG, HexColorTGrayDarker},
    {T_TAB_ITEM_ACTIVE, HexColorTGrayDarker},
    {T_TAB_ITEM_INACTIVE, HexColorStatus},

    {T_BTN_BG, HexColorTGrayDarker},
    {T_BTN_HVR_BG, HexColorTGrayDarker},
    {T_BTN_CLK_BG, HexColorStatus},
    {T_BTN_BORDER, HexColorVWhite},
    {T_BTN_CORNER, HexColorVWhite},
    {T_BTN_SHADOW, HexColorBlack},
    {T_BTN_BRDR_THICK, 2},
    {T_BTN_BRDR_CANCEL, HexColorTPink},

    {T_TOGGLE_BRDR, HexColorVGreen},
    {T_TOGGLE_CLKD_BRDR, HexColorVWhite},
    {T_TOGGLE_BG, HexColorTGrayDarker},

    {T_DTOOL_ICON_BG, HexColorVGreen},
    {T_DTOOL_ICON_FG, HexColorVWhite},
    {T_LAYER_BRDR, HexColorTGrayLighter},
    {T_LAYER_ACTIVE_BRDR, HexColorVWhite},
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

    {VALUEBOX, TEXT_COLOR_NORMAL, HexColorVWhite},
    {TEXTBOX, TEXT_COLOR_NORMAL, HexColorVWhite},
    {TEXTBOX, TEXT_COLOR_FOCUSED, HexColorVWhite},
    {TEXTBOX, TEXT_COLOR_PRESSED, HexColorVWhite},
    {TEXTBOX, BASE_COLOR_NORMAL, HexColorStatus},
    {TEXTBOX, BASE_COLOR_PRESSED, HexColorStatus},
    {TEXTBOX, BASE_COLOR_FOCUSED, HexColorStatus},
    {TEXTBOX, BORDER_COLOR_NORMAL, HexColorTGrayLightest},
    {TEXTBOX, BORDER_COLOR_FOCUSED, HexColorTGrayLighter},
    {TEXTBOX, BORDER_COLOR_PRESSED, HexColorTGrayLightest},
    {LABEL, TEXT_COLOR_NORMAL, HexColorVWhite},
    {LABEL, TEXT_COLOR_FOCUSED, HexColorVWhite},
    {LABEL, TEXT_COLOR_PRESSED, HexColorVWhite},

};

// ============== RAGUI THEME END ==============

void LoadAppIcon() { 

	Image icon = LoadImageFromMemory(".png", LOGO_PNG_DATA, LOGO_PNG_SIZE);
	SetWindowIcon(icon);
	UnloadImage(icon);
}

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
