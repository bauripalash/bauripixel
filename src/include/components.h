#ifndef COMPONENETS_H
#define COMPONENETS_H

#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/drawtools.h"
#include "../include/utils.h"
#include <stdbool.h>

bool BpInputSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit,
    bool *clicked, bool *hover
);
bool BpSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit,
    bool *sliderHover
);

bool BpDToolButton(
    Rectangle bounds, DrawTool *active, bool *showOther, int num,
    const DrawTool *tools, Texture2D *icons
);

bool BpToggleButton(Rectangle bounds, int thick, bool active);
bool BpSimpleButton(Rectangle bounds, int thick);

int BpTabItem(
    Rectangle bounds, const char *name, int index, float tabWidth, bool active
);

// FUTURE: Custom Brush | Using DToolInfo as for prototype
void BpBrushShapeButton(Rectangle bounds, BrushShape *shape, Texture2D *icons);

void BpPanelOnlyBorder(Rectangle bounds, int thick);
void BpPanelBorder(Rectangle bounds, int thick);
void BpPanelNoBorder(Rectangle bounds);

void BpSimplePanel(Rectangle bounds, int thick, Side border, Side shadow);
void BpMenuBarPanel(Vector2 position, float width, int items, float roundness);
#endif
