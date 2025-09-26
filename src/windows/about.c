#include "../include/windows/about.h"
#include "../include/components.h"
#include <stdbool.h>

#define ABT_DEF_WIDTH  500
#define ABT_DEF_HEIGHT 300

#define ABT_MARGIN_TB  30
#define ABT_LBL_H      30

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

float labelCntrX(float x, float w, const char *str) {
    Font f = GuiGetFont();
    int fs = f.baseSize;
    int sw = GuiGetTextWidth(str) / 2.0f;
    return (x + w);
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

        int ogAlign = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
        int ogFs = GuiGetStyle(DEFAULT, TEXT_SIZE);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

        Rectangle lRect = {
            bounds.x, bounds.y + WINDOW_TOP_HEIGHT + ABT_MARGIN_TB,
            bounds.width, ABT_LBL_H
        };

        const char titleStr[] = "BauriPixel";

        GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
        GuiLabel(lRect, titleStr);
        lRect.y += lRect.height + 5;
        GuiSetStyle(DEFAULT, TEXT_SIZE, ogFs);
        GuiLabel(lRect, "v0.1.0");
        lRect.y += lRect.height + 5;
        GuiLabel(lRect, "Simple & Lightweight Pixel Art Editor");
        lRect.y += lRect.height + 5;
        GuiLabel(lRect, "Developed by, Palash Bauri");
        lRect.y += lRect.height + 5;
        GuiLabel(
            lRect,
            GuiIconText(
                ICON_HEART, "In loving memory of my mother, Mukti Rani Sinha"
            )
        );

        GuiSetStyle(LABEL, TEXT_ALIGNMENT, ogAlign);
        GuiSetStyle(DEFAULT, TEXT_SIZE, ogFs);
    }

    return result;
}
