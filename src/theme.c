#include "include/theme.h"
#include "include/colors.h"
#include "include/utils.h"

// clang-format off
#include <stdlib.h>
#include <string.h>

#include "external/raylib/raygui.h"
#include "gen/biosfontii.h"
#include "include/options.h"
// clang-format on

static const BpOptThemeProp defaultThemeOpts[] = {
    {T_PANEL_BG, BpHexColorCBlack},
    {T_PANEL_BORDER, BpHexColorCOrange},
    {T_PANEL_BORDER_THICK, 2},

};

void LoadDefaultTheme(void) {
    int defaultOptCount = ArrCount(defaultThemeOpts);
    for (int i = 0; i < defaultOptCount; i++) {
        OptThemeSet(defaultThemeOpts[i].key, defaultThemeOpts[i].value);
    }
}
void LoadDefaultAppFont(void) { GuiLoadStyleBIOSfontII(); }
void LoadAppIcon(void) {}
