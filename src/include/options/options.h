#ifndef OPTIONS_H
#define OPTIONS_H

#define T_INVALID_RETURN -9998

typedef enum OptThemeKey {
    T_SCROLLBAR_BG = 0,
    T_SCROLLBAR_FG,
    T_SLIDER_BG,
    T_SLIDER_FG,

    T_MAX_THEME_KEYS
} OptThemeKey;

typedef struct OptThemeProp {
    OptThemeKey key;
    int value;
} OptThemeProp;

#define ValidOptTheme(v) (v == T_INVALID_RETURN)
void OptThemeSet(OptThemeKey key, int value);
int OptThemeGet(OptThemeKey key);

#endif
