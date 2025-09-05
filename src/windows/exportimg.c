#include "../include/windows/exportimg.h"
#include "../include/components.h"
#include <stdbool.h>
#include <string.h>

WExportImgState NewWExportImage() {
    WExportImgState expi = {0};
    expi.p = NewWinProp();
    expi.p.b.width = 300;
    expi.p.b.height = 200;
    strcpy(expi.exportPath, "");
    expi.editExportPath = false;

    return expi;
}

static void updateBounds(WExportImgState *state) {
    if (!state->p.customPos) {
        float winH = GetScreenHeight();
        float winW = GetScreenWidth();

        state->p.b.x = (winW - state->p.b.width) / 2.0f;
        state->p.b.y = (winH - state->p.b.height) / 2.0f;
    }
}

#define EX_MARGIN_LR 10
#define LABEL_WIDTH  100
#define LABEL_HEIGHT 30

WinStatus WExportImg(WExportImgState *state) {
    WinStatus result = WIN_NONE;

    if (state->p.active) {
        updateBounds(state);
        Rectangle bounds = state->p.b;

        if (BpSimpleWindow(
                &state->p.b, "Export as Image", &state->p.drag, &state->p.resize
            )) {
            result = WIN_CLOSE;
        }
    }

    return result;
}
