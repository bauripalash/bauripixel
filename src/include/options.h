#ifndef BAURIPIXEL_OPTIONS_H
#define BAURIPIXEL_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_THEME_PROP_VALUE -9999

typedef enum BpOptThemeKey {
    T_PANEL_BG,
    T_PANEL_BORDER,
    T_PANEL_BORDER_THICK,
    T_MAX_THEME_KEYS
} BpOptThemeKey;

typedef struct BpOptThemeProp {
    BpOptThemeKey key;
    int value;
} BpOptThemeProp;

void OptThemeSet(BpOptThemeKey key, int value);
int OptThemeGet(BpOptThemeKey key);
int OptThemeGetSet(BpOptThemeKey key, int value);

#ifdef __cplusplus
}
#endif

#endif
