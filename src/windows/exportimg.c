#include "../include/windows/exportimg.h"
#include "../include/components.h"
#include "../include/nativedlg.h"
#include "../include/options.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define EXPO_DEF_WIDTH  500
#define EXPO_DEF_HEIGHT 200

static const char InitLayerNames[] = "All Layers;Visible Layers";

WExportImgState NewWExportImage() {
    WExportImgState expi = {0};
    expi.p = NewWinProp();
    expi.p.b.width = EXPO_DEF_WIDTH;
    expi.p.b.height = EXPO_DEF_HEIGHT;
    strcpy(expi.exportPath, "");
    strcpy(expi.exportFilename, "");
    expi.editExportPath = false;
    expi.layerlist = NULL;

    return expi;
}

void FreeWExportImg(WExportImgState *state) {
    if (state->layerNames != NULL) {
        free(state->layerNames);
        state->layerNames = NULL;
    }
}

void SetupWExportImage(WExportImgState *state) {
    if (state->layerlist != NULL) {
        if (state->layerNames != NULL) {
            free(state->layerNames);
        }

        int lcount = state->layerlist->count;
        int nameLen = strlen(InitLayerNames);
        for (int i = 0; i < lcount; i++) {
            nameLen += strlen(state->layerlist->layers[i]->name) + 1;
        }

        state->layerNames = (char *)malloc(sizeof(char) * nameLen + 1);
        if (state->layerNames == NULL) {
            state->p.active = false; // error handle?
        }

        int textpos = 0;
        TextAppend(state->layerNames, InitLayerNames, &textpos);

        for (int i = 0; i < lcount; i++) {
            LayerObj *l = state->layerlist->layers[i];
            TextAppend(state->layerNames, TextFormat(";%s", l->name), &textpos);
        }
    }
    state->resizeValue = 2;
    state->p.customPos = false;
}

static void updateBounds(WExportImgState *state) {
    if (!state->p.customPos) {
        float winH = GetScreenHeight();
        float winW = GetScreenWidth();

        state->p.b.x = (winW - state->p.b.width) / 2.0f;
        state->p.b.y = (winH - state->p.b.height) / 2.0f;
    }
}

#define EXPO_MARGIN_LR 10
#define EXPO_MARGIN_TB 10
#define LABEL_WIDTH    100
#define LABEL_HEIGHT   30
#define BTN_SIZE       80

static int active = 0;
WinStatus WExportImg(WExportImgState *state) {
    WinStatus result = WIN_NONE;

    if (state->p.active && state->layerlist != NULL) {
        updateBounds(state);
        Rectangle bounds = state->p.b;

        bool winres = BpSimpleWindow(
            &state->p.b, "Export as Image", &state->p.drag, &state->p.resize
        );

        if (state->p.resize || state->p.drag) {
            state->p.customPos = true;
            state->p.b.width = ClampMin(state->p.b.width, EXPO_DEF_WIDTH);
            state->p.b.height = ClampMin(state->p.b.height, EXPO_DEF_HEIGHT);
        }
        bounds = state->p.b;

        if (winres) {
            result = WIN_CLOSE;
        }

        float ogWidth = bounds.width - EXPO_MARGIN_LR * 2;
        Rectangle rect = {
            bounds.x + EXPO_MARGIN_LR,
            bounds.y + WINDOW_TOP_HEIGHT + EXPO_MARGIN_TB, ogWidth, LABEL_HEIGHT
        };

        GuiLabel(rect, "Export to");
        // rect.y += LABEL_HEIGHT;
        rect.x += LABEL_WIDTH;
        rect.width -= BTN_SIZE + EXPO_MARGIN_LR + LABEL_WIDTH;
        BpTextBox(rect, state->exportFilename, 512, &state->editExportPath);
        rect.width = BTN_SIZE;
        rect.x = (bounds.x + bounds.width) - EXPO_MARGIN_LR - rect.width;
        if (BpTextButton(rect, "Browse")) {
            BpSaveFileDialog(
                "Export to", state->exportPath, "*.png;*.jpg;*.jpeg;*.gif", NULL
            );
            strcpy(state->exportFilename, GetFileName(state->exportPath));
        }

        rect.y += LABEL_HEIGHT + EXPO_MARGIN_TB;
        rect.x = bounds.x + EXPO_MARGIN_LR;
        rect.width = ogWidth;
        GuiLabel(rect, "Resize :");

        rect.y += LABEL_HEIGHT + EXPO_MARGIN_LR;
        GuiLabel(rect, "Layers : ");

        rect.width = BTN_SIZE;
        rect.y = (bounds.y + bounds.height) - rect.height - EXPO_MARGIN_TB;
        rect.x = (bounds.x + bounds.width) - EXPO_MARGIN_LR - BTN_SIZE;

        int ogBorder =
            OptThemeGetSet(T_TOGGLE_BRDR, OptThemeGet(T_BTN_BRDR_CANCEL));
        if (BpTextButton(rect, "Cancel")) {
            result = WIN_CANCEL;
        }
        OptThemeSet(T_TOGGLE_BRDR, ogBorder);

        rect.x -= BTN_SIZE + EXPO_MARGIN_LR;
        if (BpTextButton(rect, "Export")) {
            result = WIN_OK;
        }

        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

        // Dropdowns (to maintain Z index); Bottom to up;

        BpDropdownBox(
            (Rectangle){bounds.x + EXPO_MARGIN_LR + LABEL_WIDTH,
                        bounds.y + LABEL_HEIGHT * 3 + EXPO_MARGIN_TB * 3,
                        ogWidth - LABEL_WIDTH, LABEL_HEIGHT},
            state->layerNames, &state->expoLayerValue, &state->editExpoLayer
        );

        BpDropdownBox(
            (Rectangle){bounds.x + EXPO_MARGIN_LR + LABEL_WIDTH,
                        bounds.y + LABEL_HEIGHT * 2 + EXPO_MARGIN_TB * 2,
                        ogWidth - LABEL_WIDTH, LABEL_HEIGHT},
            "25%;50%;100%;200%;300%;400%;500%;600%;700%;800%;900%;1000%",
            &state->resizeValue, &state->editResize
        );
    }

    return result;
}
