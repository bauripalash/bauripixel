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
    {T_PANEL_BG, BpHexColorStatus},     {T_PANEL_BORDER, BpHexColorBlack},
    {T_PANEL_BORDER_THICK, 2},          {T_SCROLLBAR_BG, BpHexColorVGray},
    {T_SCROLLBAR_FG, BpHexColorVGreen},
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
