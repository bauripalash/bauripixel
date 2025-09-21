#ifndef COMPONENETS_H
#define COMPONENETS_H

#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/drawtools.h"
#include "../include/frames.h"
#include "../include/utils.h"
#include <stdbool.h>

#define WINDOW_TOP_HEIGHT 30

bool BpInputSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit,
    bool *clicked, bool *hover
);
bool BpSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit,
    bool *sliderHover
);

bool BpScrollPanel(
    Rectangle bounds, Rectangle content, Vector2 *scroll, Rectangle view,
    bool *hDrag, bool *vDrag, const Rectangle *zone
);
bool BpTextBox(Rectangle bounds, char *text, int textSize, bool *edit);
bool BpDropdownBox(
    Rectangle bounds, const char *options, int *selected, bool *clicked
);

bool BpDToolButton(
    Rectangle bounds, DrawTool *active, bool *showOther, int num,
    const DrawTool *tools, Texture2D *icons
);

bool BpToggleButton(Rectangle bounds, int thick, bool active);
bool BpTextButton(Rectangle bounds, const char *text);
bool BpSimpleButton(Rectangle bounds, int thick);
bool BpLabelButton(Rectangle bounds, const char *text);
bool BpTopMenuBtn(Rectangle bounds, const char *text, bool active);

int BpTabItem(
    Rectangle bounds, const char *name, int index, float tabWidth, bool active
);

// FUTURE: Custom Brush | Using DToolInfo as for prototype
void BpBrushShapeButton(Rectangle bounds, BrushShape *shape, Texture2D *icons);

void BpPanelOnlyBorder(Rectangle bounds, int thick);
void BpPanelOnlyClrBorder(Rectangle bounds, int thick, Color color);
void BpPanelBorder(Rectangle bounds, int thick);
void BpPanelNoBorder(Rectangle bounds);

void BpSimplePanel(Rectangle bounds, int thick, Side border, Side shadow);
void BpMenuBarPanel(Vector2 position, float width, int items, float roundness);
bool BpFramePrevBox(Rectangle bounds, FrameObj *frame, bool preview);
bool BpFramePrevActive(Rectangle bounds, FrameObj *frame, bool preview);
// Windows

// Returns true on close
bool BpSimpleWindow(
    Rectangle *bounds, const char *title, bool *drag, bool *resize
);
#endif
