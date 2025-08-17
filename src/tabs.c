#include "include/tabs.h"
#include "external/raylib.h"
#include "external/stb/stb_ds.h"
#include "include/colors.h"
#include "include/defaults.h"
#include "include/layers.h"
#include "include/widgets/canvas.h"
#include "include/widgets/colorbar.h"
#include "include/widgets/drawtoolbar.h"
#include "include/widgets/layerbar.h"
#include "include/widgets/menubar.h"
#include "include/widgets/statusbar.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define TOP_WIN_MARGIN    80
#define TAB_PANEL_MARGIN  8
#define TAB_HEADER_WIDTH  80
#define TAB_HEADER_HEIGHT 30

#define PALETTE_COUNT     32
// AxulArt 32 color Palette Palette by AxulArt
// https://lospec.com/palette-list/axulart-32-color-palette
static const int ColorPaletteDefault[PALETTE_COUNT] = {
    0x000000, 0xfcfcfc, 0xc4c7ee, 0x9a8fe0, 0x635d96, 0x292f65, 0x1b1d34,
    0xffe3ae, 0xcdbbab, 0xa6858f, 0xcf5d8b, 0x964968, 0xffb482, 0xdd867d,
    0xb2696f, 0xf6c65e, 0xe49057, 0xc46833, 0xb0d07e, 0x66aa5d, 0x52b5ab,
    0x2a8379, 0x1c5659, 0x7be1f6, 0x589ffc, 0x5069e4, 0x2e44ae, 0x8056d4,
    0x5a3b96, 0xffbae1, 0xe687c5, 0xa759b9,
};

TabStateObj *NewTabState(Rectangle panel, int w, int h) {
    TabStateObj *ts = malloc(sizeof(TabStateObj));
    if (ts == NULL) {
        return NULL;
    }

    ts->cvs = NewCanvas(w, h);
    ts->cb = NewColorBar();
    ts->dtb = NewDrawToolBar();
    ts->lb = NewLayerBar();

    ts->cvs.prop.active = false;
    ts->cb.prop.active = false;
    ts->dtb.prop.active = false;
    ts->lb.p.active = false;
    ts->lb.previewBg = &ts->cvs.bgTxt;

    return ts;
}

void FreeTabState(TabStateObj *state) {
    if (state == NULL) {
        return;
    }

    // * Free Any States Which allocates memmory *

    FreeDrawToolBar(&state->dtb);
    FreeCanvas(&state->cvs);
    free(state);
}

void SetupTabData(TabObj *tab, MenuBarState *menu, StatusBarState *status) {
    if (tab->setupDone) {
        return;
    }
    SyncTabData(tab, menu, status);
    tab->state->dtb.prop.active = true;
    tab->state->cb.prop.active = true;
    tab->state->lb.p.active = true;
    tab->state->cvs.prop.active = true;
    SetCanvasAnchor(
        &tab->state->cvs,
        (Vector2){tab->state->dtb.toolsRect.x + tab->state->dtb.toolsRect.width,
                  tab->state->dtb.optRect.y + tab->state->dtb.optRect.height},
        (Vector2){tab->state->cb.prop.bounds.x, tab->state->lb.p.bounds.y}
    );
    CenterAlignCanvas(&tab->state->cvs);

    for (int i = 0; i < PALETTE_COUNT; i++) {
        AddColorToTab(tab, GetColor(BpRGB2SolidRGBA(ColorPaletteDefault[i])));
    }

    tab->state->cb.currentColor = tab->colors[0];
    tab->state->cb.currentIndex = 0;

    tab->setupDone = true;
}

void SyncTabData(TabObj *tab, MenuBarState *menu, StatusBarState *status) {

    tab->state->cvs.curTool = tab->state->dtb.currentTool;
    tab->state->cvs.brushSize = tab->state->dtb.brushSize;
    tab->state->cvs.brushShape = tab->state->dtb.brushShape;
    tab->state->cb.colors = tab->colors;
    tab->state->cb.colorCount = tab->colorCount;
    tab->state->lb.curLayer = tab->curLayer;
    tab->state->cvs.curLayer = tab->curLayer;

    // 5 is menubar margin
    float menuHeight = DEF_MENUBAR_HEIGHT;

    tab->tabPanel =
        (Rectangle){TAB_PANEL_MARGIN,
                    menuHeight + TAB_PANEL_MARGIN + TAB_HEADER_HEIGHT,
                    GetScreenWidth() - TAB_PANEL_MARGIN * 2, 0};

    float statusHeight = DEF_STATUSBAR_HEIGHT;
    float statusY = GetScreenHeight() - statusHeight - TAB_PANEL_MARGIN;

    tab->tabPanel.height = statusY - tab->tabPanel.y - TAB_PANEL_MARGIN;

    Rectangle panel = tab->tabPanel;
    SetDrawToolBarAnchor(
        &tab->state->dtb, (Vector2){panel.x, panel.y},
        (Vector2){panel.x + panel.width, panel.y + panel.height}
    );

    SetColorBarAnchor(
        &tab->state->cb,
        (Vector2){-1,
                  tab->state->dtb.optRect.y + tab->state->dtb.optRect.height},
        (Vector2){tab->tabPanel.x + tab->tabPanel.width, -1}
    );

    SetLayerBarAnchor(
        &tab->state->lb,
        (Vector2){
            tab->state->dtb.toolsRect.x + tab->state->dtb.toolsRect.width,
        },
        (Vector2){tab->state->cb.prop.bounds.x, panel.y + panel.height}
    );

    if (CurrentColorChanged(&tab->state->cb)) {
        tab->state->cvs.current = tab->state->cb.currentColor;
    }

    if (tab->state->lb.selLayer->index != tab->curLayer->index) {
        tab->curLayer = tab->state->lb.selLayer;
    }

    if (tab->setupDone) {

        UpdateCanvasAnchor(
            &tab->state->cvs,
            (Vector2){
                tab->state->dtb.toolsRect.x + tab->state->dtb.toolsRect.width,
                tab->state->dtb.optRect.y + tab->state->dtb.optRect.height
            },
            (Vector2){tab->state->cb.prop.bounds.x, tab->state->lb.p.bounds.y}
        );
    }
}

void AddColorToTab(TabObj *tab, Color color) {
    if (tab == NULL) {
        return;
    }

    arrpush(tab->colors, color);
    tab->colorCount++;
}

TabObj *NewTabObj(int w, int h) {
    TabObj *t = malloc(sizeof(TabObj));
    if (t == NULL) {
        return NULL;
    }

    float menuHeight = DEF_MENUBAR_HEIGHT;

    t->tabPanel = (Rectangle){TAB_PANEL_MARGIN,
                              menuHeight + TAB_PANEL_MARGIN + TAB_HEADER_HEIGHT,
                              GetScreenWidth() - TAB_PANEL_MARGIN * 2, 0};

    float statusY = GetScreenHeight() - DEF_STATUSBAR_HEIGHT;

    t->tabPanel.height = statusY - t->tabPanel.y - TAB_PANEL_MARGIN;

    t->state = NewTabState(t->tabPanel, w, h);
    if (t->state == NULL) {
        free(t);
        return NULL;
    }

    t->setupDone = false;

    t->layers = NewLayerList(w, h);

    if (t->layers == NULL) {
        FreeTabState(t->state);
        free(t);
        return NULL;
    }

    t->curLayer = t->layers->layers[0]; // layer list will have atleast one
                                        // layer after creation;

    UpdateCanvasLayers(&t->state->cvs, t->layers, t->curLayer);
    t->state->lb.list = t->layers;
    t->state->lb.curLayer = t->curLayer;
    t->state->lb.selLayer = t->curLayer;
    t->index = 0;

    t->canvasWidth = w;
    t->canvasHeight = h;

    t->state->lb.gridSize = (Vector2){w, h};

    t->bg = BLANK;
    t->colorCount = 0;
    t->colors = NULL; // fill be handled via stb_ds

    t->filepath = NULL; // will be set later
    t->state->dtb.maxBrushSize = (int)fmaxf((float)w, (float)h);

    return t;
}

void FreeTabObj(TabObj *tab) {
    if (tab == NULL) {
        return;
    }

    if (tab->colors != NULL) {
        for (int i = 0; i < tab->colorCount; i++) {
            arrpop(tab->colors);
        }

        arrfree(tab->colors);
    }

    FreeLayerList(tab->layers);

    if (tab->filepath != NULL) {
        free(tab->filepath);
    }

    FreeTabState(tab->state);

    free(tab);
}

TabList *NewTabList() {
    TabList *tl = malloc(sizeof(TabList));
    tl->tabs = NULL; // will be handled by stb_ds
    tl->count = 0;

    return tl;
}

void FreeTabList(TabList *list) {
    if (list == NULL || list->tabs == NULL) {
    }
    int count = arrlen(list->tabs);

    while (count > 0) {
        FreeTabObj(list->tabs[count - 1]);
        arrpop(list->tabs);

        count = arrlen(list->tabs);
    }

    arrfree(list->tabs);
    free(list);
}

void AddToTabList(TabList *list, TabObj *tab) {
    if (list == NULL || tab == NULL) {
        return;
    }

    arrput(list->tabs, tab);
    list->count = arrlen(list->tabs);
}
void RemoveIdxTabList(TabList *list, int index) { return; }
