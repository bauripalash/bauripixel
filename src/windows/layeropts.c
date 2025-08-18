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
    lo.name = NULL;
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

        if (BpSimpleWindow(
                state->p.b, GuiIconText(ICON_LAYERS, "Layer Settings")
            )) {
            state->p.active = false;
        }
    }
}

void FreeWLayerOpts(WLayerOptsState *state) { return; }
