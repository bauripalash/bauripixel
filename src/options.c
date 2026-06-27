/*
 * options.c - Application Theme and Values Setup and Customization
 * Copyright (C) <year>  <name of author>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
