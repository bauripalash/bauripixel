#ifndef COMPONENETS_H
#define COMPONENETS_H

#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/drawtools.h"
#include <stdbool.h>

bool BpInputSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit,
    bool *clicked
);
bool BpSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit
);

typedef struct DToolInfo {
    DrawTool tool;
    Image img;
    Texture txt;
} DToolInfo;

bool BpDummyToogleButton(Rectangle bounds, bool active);
DrawTool BpToolButton(
    Rectangle bounds, DrawTool active, bool *showOther, int num,
    const DToolInfo *tools
);

// FUTURE: Custom Brush | Using DToolInfo as for prototype
void BpBrushShapeButton(
    Rectangle bounds, BrushShape *shape, const DToolInfo *tools
);

#endif
