#ifndef OPTIONS_H
#define OPTIONS_H

#define T_INVALID_RETURN -9998

typedef enum OptThemeKey {
    T_SCROLLBAR_BG = 0,
    T_SCROLLBAR_FG,
    T_SCROLLBAR_HVR_BG,
    T_SCROLLBAR_HVR_FG,
    T_SCROLLBAR_CLK_BG,
    T_SCROLLBAR_CLK_FG,
    T_SLIDER_BG,
    T_SLIDER_BORDER,
    T_SLIDER_FG,
    T_SLIDER_TEXT_PRE,
    T_SLIDER_TEXT_POST,

    T_ISLIDER_BG,
    T_ISLIDER_FG,

    T_PANEL_SHADOW,
    T_PANEL_BORDER,
    T_PANEL_BG,
    T_APP_BG,
    T_TAB_PANEL_BG,
    T_TAB_ITEM_ACTIVE,
    T_TAB_ITEM_INACTIVE,

    T_BTN_BG,
    T_BTN_HVR_BG,
    T_BTN_CLK_BG,
    T_BTN_BORDER,
    T_BTN_CORNER,
    T_BTN_SHADOW,
    T_BTN_BRDR_THICK,

    T_TOGGLE_BG,
    T_TOGGLE_BRDR,
    T_TOGGLE_CLKD_BRDR,

    T_DTOOL_ICON_FG,
    T_DTOOL_ICON_BG,

    T_LAYER_BRDR,
    T_LAYER_ACTIVE_BRDR,

    T_MAX_THEME_KEYS
} OptThemeKey;

typedef struct OptThemeProp {
    OptThemeKey key;
    int value;
} OptThemeProp;

#define ValidOptTheme(v) (v == T_INVALID_RETURN)
void OptThemeSet(OptThemeKey key, int value);
int OptThemeGetSet(OptThemeKey key, int value);
int OptThemeGet(OptThemeKey key);

#endif
