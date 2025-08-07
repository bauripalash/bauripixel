#include "include/tabs.h"
#include "external/raylib.h"
#include "external/stb/stb_ds.h"
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
#define TAB_PANEL_MARGIN  10
#define TAB_HEADER_WIDTH  80
#define TAB_HEADER_HEIGHT 24

TabStateObj *NewTabState(int w, int h) {
    TabStateObj *ts = malloc(sizeof(TabStateObj));
    if (ts == NULL) {
        return NULL;
    }

    ts->cvs = NewCanvas(w, h);

    ts->cb = NewColorBar();
    ts->dtb = NewDrawToolBar();
    ts->lb = NewLayerBar();

    ts->cvs.prop.active = true;
    ts->cb.prop.active = true;
    ts->dtb.prop.active = true;
    ts->lb.p.active = true;
    ts->lb.previewBg = &ts->cvs.bgTxt;

    SetDrawToolBarAnchor(&ts->dtb, (Vector2){-1, -1}, (Vector2){-1, 30});

    SetColorBarAnchor(&ts->cb, (Vector2){-1, TOP_WIN_MARGIN}, (Vector2){0, 0});
    SetCanvasAnchor(
        &ts->cvs, (Vector2){ts->dtb.prop.bounds.width, TOP_WIN_MARGIN},
        (Vector2){ts->cb.prop.bounds.x, ts->lb.p.bounds.y}
    );

    SetLayerBarAnchor(
        &ts->lb, (Vector2){ts->cvs.prop.bounds.x, -1},
        (Vector2){ts->cvs.prop.bounds.x + ts->cvs.prop.bounds.width, 30}
    );

    ts->dtb.anchor.x = 0;
    ts->dtb.anchor.y = TOP_WIN_MARGIN + CANVAS_MARGIN_TB;
    ts->dtb.optAnchor.y = 20;

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

void SyncTabData(TabObj *tab, MenuBarState *menu, StatusBarState *status) {
    tab->state->cvs.curTool = tab->state->dtb.currentTool;
    tab->state->cvs.brushSize = tab->state->dtb.brushSize;
    tab->state->cvs.brushShape = tab->state->dtb.brushShape;
    tab->state->cb.colors = tab->colors;
    tab->state->cb.colorCount = tab->colorCount;
    tab->state->lb.curLayer = tab->curLayer;
    tab->state->cvs.curLayer = tab->curLayer;

    tab->tabPanel = (Rectangle){TAB_PANEL_MARGIN,
                                menu->prop.bounds.y + menu->prop.bounds.height +
                                    TAB_PANEL_MARGIN + TAB_HEADER_HEIGHT,
                                GetScreenWidth() - TAB_PANEL_MARGIN * 2, 0};

    tab->tabPanel.height =
        status->prop.bounds.y - tab->tabPanel.y - TAB_PANEL_MARGIN;

    SetDrawToolBarAnchor(
        &tab->state->dtb, (Vector2){tab->tabPanel.x, tab->tabPanel.y},
        (Vector2){-1, -1}
    );

    if (CurrentColorChanged(&tab->state->cb)) {
        tab->state->cvs.current = tab->state->cb.currentColor;
    }

    UpdateCanvasAnchor(
        &tab->state->cvs, (Vector2){-1, tab->tabPanel.y},
        (Vector2){tab->state->cb.prop.bounds.x - TAB_PANEL_MARGIN * 2,
                  tab->state->lb.p.bounds.y}
    );

    SetLayerBarAnchor(
        &tab->state->lb,
        (Vector2){
            tab->state->cvs.prop.bounds.x,
        },
        (
            Vector2
        ){tab->state->cvs.prop.bounds.x + tab->state->cvs.prop.bounds.width, -1}
    );

    if (tab->state->lb.selLayer->index != tab->curLayer->index) {
        tab->curLayer = tab->state->lb.selLayer;
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

    t->state = NewTabState(w, h);

    if (t->state == NULL) {
        free(t);
        return NULL;
    }

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
    t->tabPanel = (Rectangle){0};

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
