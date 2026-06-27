#include "options.h"

static int ThemeOptions[T_MAX_THEME_KEYS];

void OptThemeSet(BpOptThemeKey key, int value) {
    if (key >= 0 && key < T_MAX_THEME_KEYS) {
        ThemeOptions[key] = value;
    }
}

int OptThemeGet(BpOptThemeKey key) {
    if (key >= 0 && key < T_MAX_THEME_KEYS) {
        return ThemeOptions[key];
    }

    return INVALID_THEME_PROP_VALUE;
}

int OptThemeGetSet(BpOptThemeKey key, int value) {
    int og = OptThemeGet(key);
    if (og == INVALID_THEME_PROP_VALUE) {
        return INVALID_THEME_PROP_VALUE;
    }

    OptThemeSet(key, value);

    return og;
}
