#include "../include/windows/layeropts.h"
#include "../include/components.h"
#include <stdbool.h>
#include <string.h>

WLayerOptsState NewWLayerOpts() {
    WLayerOptsState lo = {0};
    lo.p = NewWinProp();
    lo.p.b.width = 500;
    lo.p.b.height = 200;
    lo.layer = NULL;
    strcpy(lo.name, "");
    lo.editName = false;
    return lo;
}

static void updateBounds(WLayerOptsState *state) {
    float winH = GetScreenHeight();
    float winW = GetScreenWidth();
    state->p.b.x = (winW - state->p.b.width) / 2.0f;
    state->p.b.y = (winH - state->p.b.height) / 2.0f;
}

void WLayerOpts(WLayerOptsState *state) {
    if (state->p.active && state->layer != NULL) {
        updateBounds(state);
        Rectangle bounds = state->p.b;
        if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        if (BpSimpleWindow(
                bounds, GuiIconText(ICON_LAYERS, "Layer Settings")
            )) {
            state->p.active = false;
        }

        float px = bounds.x + 10;
        float py = bounds.y + 30 + 5;

        GuiLabel((Rectangle){px, py, 100, 30}, "Layer Name:");
        px += 100 + 10;
        GuiTextBox((Rectangle){px, py, 50, 30}, state->name, 100, true);
    }
}

void FreeWLayerOpts(WLayerOptsState *state) { return; }
