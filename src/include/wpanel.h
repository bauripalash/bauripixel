#ifndef BAURIPIXEL_WIDGET_PANEL_H
#define BAURIPIXEL_WIDGET_PANEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../external/raylib/raylib.h"
#include "widget.h"

#define DEFAULT_PANEL_BORDER_THICKNESS 2

typedef struct WPanel {
    BpWidget w;
    Color background;
    Color border;
    int borderThickness;
} WPanel;

WPanel NewWPanel(int width, int height);
void PanelSetColor(WPanel *panel, Color bg, Color border);
void PanelSetAnchor(WPanel *panel, Vector2 anchor, Vector2 stopper);
#ifdef __cplusplus
}
#endif

#endif
