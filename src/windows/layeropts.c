#include "../include/windows/layeropts.h"
#include <stdbool.h>
#include <string.h>

WLayerOptsState NewWLayerOpts() {
    WLayerOptsState lo = {0};
    lo.p = NewWinProp();
    lo.layer = NULL;
    lo.name = NULL;
    lo.editName = false;
    return lo;
}

void WLayerOpts(WLayerOptsState *state) {
    if (state->p.active && state->layer != NULL) {
        DrawRectangleRec(state->p.b, WHITE);
    }
}

void FreeWLayerOpts(WLayerOptsState *state) { return; }
