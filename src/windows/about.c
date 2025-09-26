#include "../include/windows/about.h"
#include "../include/components.h"
#include <stdbool.h>
#include <string.h>

#define ABT_DEF_WIDTH  500
#define ABT_DEF_HEIGHT 300

WAboutState NewWAbout() {
    WAboutState abt = {0};
    abt.p = NewWinProp();
    abt.p.b.width = ABT_DEF_WIDTH;
    abt.p.b.height = ABT_DEF_HEIGHT;

    return abt;
}
void FreeWAbout(WAboutState *state) { return; }

static void updateBounds(WAboutState *state) {
    if (!state->p.customPos) {
        state->p.b.x = (GetScreenWidth() - state->p.b.width) / 2.0f;
        state->p.b.y = (GetScreenHeight() - state->p.b.height) / 2.0f;
    }
}

WinStatus WAbout(WAboutState *state, double dt) {
    WinStatus result = WIN_NONE;

    if (state->p.active) {
        updateBounds(state);
        Rectangle bounds = state->p.b;
        bool winRes = BpSimpleWindow(
            &state->p.b, GuiIconText(ICON_INFO, "About BauriPixel"),
            &state->p.drag, &state->p.resize
        );

        if (RectAnyDiff(bounds, state->p.b)) {
            state->p.customPos = true;
            state->p.b.width = ClampMin(state->p.b.width, ABT_DEF_WIDTH);
            state->p.b.height = ClampMin(state->p.b.height, ABT_DEF_HEIGHT);
        }

        if (winRes) {
            result = WIN_CLOSE;
        }
    }

    return result;
}
