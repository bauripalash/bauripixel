#include "../include/colors.h"
#include "../include/wpanel.h"
#include <stdbool.h>
#include <stdlib.h>

int PanelDraw(BpWidget *base, void *ctx) {
    (void)ctx;
    if (base == NULL) return -1;

    WPanel *panel = (WPanel *)base;
    Rectangle bounds = {
        base->bounds.anchor.x, base->bounds.anchor.y,
        base->bounds.size.x - base->bounds.stopper.x,
        base->bounds.size.y - base->bounds.stopper.y
    };
    DrawRectangleRec(bounds, panel->background);
    DrawRectangleLinesEx(bounds, panel->borderThickness, panel->border);
    return 0;
}

int PanelUpdate(BpWidget *base, void *ctx) {
    (void)base;
    (void)ctx;
    return 0;
}

WPanel NewWPanel(int width, int height) {
    WPanel panel = {
        .w =
            {
                .bounds =
                    {.anchor = {0, 0},
                     .stopper = {0, 0},
                     .size = {width, height}},
                .visible = true,
                .enabled = true,
            },
        .background = BpColorCBlack,
        .border = BpColorVGreen,
        .borderThickness = DEFAULT_PANEL_BORDER_THICKNESS,
    };

    panel.w.Draw = PanelDraw;
    panel.w.Update = PanelUpdate;

    return panel;
}

void PanelSetColor(WPanel *panel, Color bg, Color border) {
    if (panel == NULL) return;
    panel->background = bg;
    panel->border = border;
}
