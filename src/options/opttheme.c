#include "../include/options/options.h"

static int ThemeOptions[T_MAX_THEME_KEYS];

void OptThemeSet(OptThemeKey key, int value) {
    if (key >= 0 && key < T_MAX_THEME_KEYS) {
        ThemeOptions[key] = value;
    }
}
int OptThemeGet(OptThemeKey key) {
    if (key >= 0 && key < T_MAX_THEME_KEYS) {
        return ThemeOptions[key];
    }

    return -9998;
}
