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

typedef struct DToolInfo {
    DrawTool tool;
    Image img;
    Texture txt;
} DToolInfo;

DrawTool BpDToolButton(
    Rectangle bounds, DrawTool active, bool *showOther, int num,
    const ToolInfo *tools
);

DrawTool BpToolButton(
    Rectangle bounds, DrawTool active, bool *showOther, int num,
    const DToolInfo *tools
);

bool BpToggleButton(Rectangle bounds, int thick, bool active);

bool BpDummyToggleButton(Rectangle bounds, bool active);
bool BpDummyFlatButton(Rectangle bounds);
bool BpSimpleButton(Rectangle bounds, int thick);

int BpTabItem(
    Rectangle bounds, const char *name, int index, float tabWidth, bool active
);

// FUTURE: Custom Brush | Using DToolInfo as for prototype
void BpBrushShapeButton(
    Rectangle bounds, BrushShape *shape, const DToolInfo *tools
);

void BpPanelBorder(Rectangle bounds, int thick);
void BpPanelNoBorder(Rectangle bounds);

void BpSimplePanel(Rectangle bounds, int thick, Side border, Side shadow);
void BpRoundedPanel(Rectangle bounds, float thick, float shadowr, bool shadow);
void BpRoundedFlatPanel(Rectangle bounds, float roundness);
void BpMenuBarPanel(Vector2 position, float width, int items, float roundness);
void BpDummyPanel(
    Rectangle bounds, float thick, float roundness, bool shadow, Vector4 borders
);
void BpDummyFlatPanel(Rectangle bounds, float thick, Vector4 borders);
#endif
