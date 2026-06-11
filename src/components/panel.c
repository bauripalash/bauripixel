#include "../external/raylib/raylib.h"
#include "../include/components.h"
#include "../include/options.h"
#include "raylib.h"

void BpPanelBorder(Rectangle bounds, int thickness) {
    Color bgColor = GetColor(OptThemeGet(T_PANEL_BG));
    Color borderColor = GetColor(OptThemeGet(T_PANEL_BORDER));
    DrawRectangleRec(bounds, bgColor);
    DrawRectangleLinesEx(bounds, thickness, borderColor);
}
void BpPanelOnlyBorder(Rectangle bounds, int thickness) {}
void BpPanelNoBorder(Rectangle bounds) {}
