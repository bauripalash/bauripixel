/*
 * options.h - Application Theme and Customization Definitions
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
    T_SCROLLBAR_BG,
    T_SCROLLBAR_FG,
    T_SCROLLBAR_HVR_FG,
    T_SCROLLBAR_CLK_FG,
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
