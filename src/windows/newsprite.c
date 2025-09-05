#include "../include/windows/newsprite.h"
#include "../include/components.h"
#include "../include/options.h"
#include <stdbool.h>
#include <string.h>

WNewSpriteState NewWNewSprite() {
    WNewSpriteState ns = {0};
    ns.p = NewWinProp();
    ns.p.b.width = 500;
    ns.p.b.height = 300;
    strcpy(ns.name, "Untitled");
    ns.editName = false;
    ns.sizeLinked = true;

    ns.width = 32;
    ns.height = 32;

    ns.editWidth = false;
    ns.hoverWidth = false;

    ns.editHeight = false;
    ns.hoverHeight = false;

    return ns;
}

void ClearWNewSpriteState(WNewSpriteState *state) {
    strcpy(state->name, "Untitled");
}

static void updateBounds(WNewSpriteState *state) {
    if (!state->p.customPos) {
        state->p.b.x = (GetScreenWidth() - state->p.b.width) / 2.0f;
        state->p.b.y = (GetScreenHeight() - state->p.b.height) / 2.0f;
    }
}

#define NS_MARGIN_LR 10
#define NS_MARGIN_TB 10
#define LABEL_WIDTH  100
#define LABEL_HEIGHT 30

#define BTN_SIZE     80

WinStatus WNewSprite(WNewSpriteState *state) {

    WinStatus result = WIN_NONE;
    if (state->p.active) {
        updateBounds(state);
        Rectangle bounds = state->p.b;

        if (BpSimpleWindow(&state->p.b, "New Sprite", NULL, NULL)) {
            if (bounds.x != state->p.b.x || bounds.y != state->p.b.y) {
                state->p.customPos = true;
            }
            result = WIN_CLOSE;
        }
        Rectangle labelRect = {
            bounds.x + NS_MARGIN_LR,
            bounds.y + WINDOW_TOP_HEIGHT + NS_MARGIN_TB, LABEL_WIDTH,
            LABEL_HEIGHT
        };
        float inputWidth = bounds.width - LABEL_WIDTH - NS_MARGIN_LR * 3;
        Rectangle inputRect = {
            labelRect.x + labelRect.width + NS_MARGIN_LR, labelRect.y,
            inputWidth, labelRect.height
        };
        GuiLabel(labelRect, "Sprite Name");
        if (GuiTextBox(
                inputRect, state->name, DEF_NAME_SIZE, state->editName
            )) {
            state->editName = !state->editName;
        }

        labelRect.y += LABEL_HEIGHT + NS_MARGIN_TB;
        inputRect.y += LABEL_HEIGHT + NS_MARGIN_TB;

        GuiLabel(labelRect, "Sprite Size");

        float sizeInputWidth =
            (inputRect.width - LABEL_HEIGHT * 2 - (NS_MARGIN_LR)) / 2.0f;

        inputRect.width = sizeInputWidth;
        if (BpInputSliderInt(
                inputRect, &state->width, 0, 100, "px", &state->editWidth,
                &state->hoverWidth
            )) {
            if (state->sizeLinked) {
                state->height = state->width;
            }
        }

        inputRect.x += inputRect.width;
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        GuiLabel(
            (Rectangle){inputRect.x, inputRect.y, LABEL_HEIGHT, LABEL_HEIGHT},
            "X"
        );

        inputRect.x += LABEL_HEIGHT;

        if (BpInputSliderInt(
                inputRect, &state->height, 0, 100, "px", &state->editHeight,
                &state->hoverHeight
            )) {
            if (state->sizeLinked) {
                state->width = state->height;
            }
        }

        inputRect.x += inputRect.width + NS_MARGIN_LR;

        if (BpLabelButton(
                (Rectangle){inputRect.x, inputRect.y, LABEL_HEIGHT,
                            LABEL_HEIGHT},
                GuiIconText(
                    state->sizeLinked ? ICON_LOCK_CLOSE : ICON_LOCK_OPEN, NULL
                )
            )) {
            state->sizeLinked = !state->sizeLinked;
        }

        inputRect.width = BTN_SIZE;
        inputRect.y =
            (bounds.y + bounds.height) - inputRect.height - NS_MARGIN_TB;
        inputRect.x = (bounds.x + bounds.width) - NS_MARGIN_LR - BTN_SIZE;

        int ogBorder =
            OptThemeGetSet(T_TOGGLE_BRDR, OptThemeGet(T_BTN_BRDR_CANCEL));
        if (BpTextButton(inputRect, "Cancel")) {
            result = WIN_CANCEL;
        }
        OptThemeSet(T_TOGGLE_BRDR, ogBorder);

        inputRect.x -= BTN_SIZE + NS_MARGIN_LR;
        if (BpTextButton(inputRect, "Create")) {
            result = WIN_OK;
        }

        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    }
    return result;
}
