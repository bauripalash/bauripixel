/*
 * theme.c - Themes Setup and Loading
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

#include "theme.h"
#include "colors.h"
#include "utils.h"

// clang-format off
#include <stdlib.h>
#include <string.h>

#include "external/raylib/raygui.h"
#include "gen/biosfontii.h"
#include "options.h"
// clang-format on

static const BpOptThemeProp defaultThemeOpts[] = {
    {T_PANEL_BG, BpHexColorStatus},
    {T_PANEL_BORDER, BpHexColorBlack},
    {T_PANEL_BORDER_THICK, 2},
    {T_SCROLLBAR_BG, BpHexColorBlack},
    {T_SCROLLBAR_FG, BpHexColorVGreen},
    {T_SCROLLBAR_HVR_FG, BpHexColorVGrayLight},
    {T_SCROLLBAR_CLK_FG, BpHexColorVGreen},
};

static const GuiStyleProp defaultThemeRGuiOpts[] = {
    {DEFAULT, TEXT_LINE_SPACING, 1},
    {DEFAULT, TEXT_SIZE, 16},
    {DEFAULT, TEXT_SPACING, 0},

    // Scrollpanel thumb
    {SLIDER, BORDER_COLOR_NORMAL, BpHexColorVGreen},
    {SLIDER, BORDER_COLOR_FOCUSED, BpHexColorVGreen},
    {SLIDER, BORDER_COLOR_PRESSED, BpHexColorVGrayLight},
};

void LoadDefaultTheme(void) {
    int defaultOptCount = ArrCount(defaultThemeOpts);
    for (int i = 0; i < defaultOptCount; i++) {
        BpOptThemeProp prop = defaultThemeOpts[i];
        OptThemeSet(prop.key, prop.value);
    }

    int defaultRGuiOptCount = ArrCount(defaultThemeRGuiOpts);
    for (int i = 0; i < defaultRGuiOptCount; i++) {
        GuiStyleProp prop = defaultThemeRGuiOpts[i];
        GuiSetStyle(prop.controlId, prop.propertyId, prop.propertyValue);
    }
}
void LoadDefaultAppFont(void) { GuiLoadStyleBIOSfontII(); }
void LoadAppIcon(void) {}
