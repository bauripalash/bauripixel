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
    lo.opacityVal = 100;
    lo.opacityClicked = false;
    return lo;
}

static void updateBounds(WLayerOptsState *state) {
    float winH = GetScreenHeight();
    float winW = GetScreenWidth();
    state->p.b.x = (winW - state->p.b.width) / 2.0f;
    state->p.b.y = (winH - state->p.b.height) / 2.0f;
}

#define LO_MARGIN_LR 10
#define LO_MARGIN_TB 10
#define LABEL_WIDTH  100
#define LABEL_HEIGHT 30

WinStatus WLayerOpts(WLayerOptsState *state) {
    WinStatus result = WIN_NONE;
    if (state->p.active && state->layer != NULL) {
        updateBounds(state);
        Rectangle bounds = state->p.b;
        if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        if (BpSimpleWindow(
                bounds, GuiIconText(ICON_LAYERS, "Layer Settings")
            )) {
            result = WIN_CLOSE;
        }

        float ogPx = bounds.x + LO_MARGIN_LR;
        Rectangle labelRect = {
            ogPx, bounds.y + WINDOW_TOP_HEIGHT + LO_MARGIN_TB, LABEL_WIDTH,
            LABEL_HEIGHT
        };

        float inputWidth = bounds.width - (LABEL_WIDTH + LO_MARGIN_LR * 4);
        Rectangle inputRect = {
            ogPx + LABEL_WIDTH + LO_MARGIN_LR, labelRect.y, inputWidth,
            LABEL_HEIGHT
        };

        GuiLabel(labelRect, "Layer Name:");
        if (GuiTextBox(inputRect, state->name, 100, state->editName)) {
            state->editName = !state->editName;
        }

        labelRect.y += LABEL_HEIGHT + LO_MARGIN_TB;
        inputRect.y = labelRect.y;

        GuiLabel(labelRect, "Opacity:");
        BpSliderInt(
            inputRect, &state->opacityVal, 0, 100, "%", &state->opacityClicked
        );

        Rectangle btnRect = {
            bounds.x + bounds.width - LO_MARGIN_LR * 2 - LABEL_WIDTH,
            bounds.y + bounds.height - LO_MARGIN_TB * 2 - LABEL_HEIGHT,
            LABEL_WIDTH, LABEL_HEIGHT
        };
        int ogAlign = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        if (BpToggleButton(btnRect, 2, false)) {
            result = WIN_OK;
        }
        GuiLabel(btnRect, "Save");

        btnRect.x -= LABEL_WIDTH + LO_MARGIN_LR * 2;
        if (BpToggleButton(btnRect, 2, false)) {
            result = WIN_CANCEL;
        }
        GuiLabel(btnRect, "Cancel");

        GuiSetStyle(LABEL, TEXT_ALIGNMENT, ogAlign);
    }

    return result;
}

void FreeWLayerOpts(WLayerOptsState *state) { return; }
