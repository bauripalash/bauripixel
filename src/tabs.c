#include "include/tabs.h"
#include "external/raylib.h"
#include "external/stb/stb_ds.h"
#include "external/tfd/tinyfiledialogs.h"
#include "include/colors.h"
#include "include/defaults.h"
#include "include/export.h"
#include "include/layers.h"
#include "include/widgets/canvas.h"
#include "include/widgets/colorbar.h"
#include "include/widgets/drawtoolbar.h"
#include "include/widgets/layerbar.h"
#include "include/widgets/menubar.h"
#include "include/widgets/statusbar.h"
#include "include/windows/exportimg.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define TOP_WIN_MARGIN    80
#define TAB_PANEL_MARGIN  8
#define TAB_HEADER_WIDTH  80
#define TAB_HEADER_HEIGHT 30

#define PALETTE_COUNT     16
// Sweetie 16 Palette by GrafxKid
// https://lospec.com/palette-list/sweetie-16
static const unsigned int ColorPaletteDefault[PALETTE_COUNT] = {
    0x1a1c2c, 0x5d275d, 0xb13e53, 0xef7d57, 0xffcd75, 0xa7f070,
    0x38b764, 0x257179, 0x29366f, 0x3b5dc9, 0x41a6f6, 0x73eff7,
    0xf4f4f4, 0x94b0c2, 0x566c86, 0x333c57,
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
    ts->eximg = NewWExportImage();

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
    FreeWExportImg(&state->eximg);
    free(state);
}

void SetupTabData(
    TabObj *tab, MenuBarState *menu, StatusBarState *status, double dt
) {
    if (tab->setupDone) {
        return;
    }
    SyncTabData(tab, menu, status, dt);
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
    tab->state->lb.curFrame = 0;

    tab->setupDone = true;
}

BpImgFmt getFormat(const char *ext) {
    if (TextIsEqual(ext, ".png")) {
        return BP_IMG_PNG;
    } else if (TextIsEqual(ext, ".jpg") || TextIsEqual(ext, ".jpeg")) {
        return BP_IMG_JPEG;
    } else if (TextIsEqual(ext, ".gif")) {
        return BP_IMG_GIF;
    }

    return BP_IMG_COUNT;
}

typedef enum {
    EXPORT_ALL = 0,
    EXPORT_VIS = 1,
    EXPORT_INDV = 2
} ExportLayerWhich;

typedef enum {
    SCALE_1x = 0,
    SCALE_05x = 1,
    SCALE_025x = 2,
    SCALE_2x = 3,
    SCALE_10x = 10 + 1, // 10x Scaling (max) (if 2x => 3 then 10x => 10 + 1)
} ExportScale;

Image getPatchedImage(TabObj *tab) {
    int whichLayer = tab->state->eximg.expoLayerValue;
    Image img = GenImageColor(tab->canvasWidth, tab->canvasHeight, BLANK);
    Rectangle imgRect = {0, 0, tab->canvasWidth, tab->canvasHeight};
    if (whichLayer == EXPORT_ALL || whichLayer == EXPORT_VIS) {
        for (int i = tab->layers->count - 1; i >= 0; i--) {
            LayerObj *layer = tab->layers->layers[i];
            if (whichLayer == EXPORT_VIS &&
                !layer->visible) { // Visible Layer Export
                continue;
            }
            ImageDraw(
                &img, layer->flist->frames[tab->curFrame]->img, imgRect,
                imgRect, WHITE
            );
        }
    } else if (whichLayer >= EXPORT_INDV) {
        // Layer name index starts from 2
        // first layer index = (whichLayer - EXPORT_INDV (2))
        int layerIndex = abs(whichLayer - EXPORT_INDV); // abs(...) -> fail safe
        LayerObj *layer = tab->layers->layers[layerIndex];
        ImageDraw(
            &img, layer->flist->frames[tab->curFrame]->img, imgRect, imgRect,
            WHITE
        );
    }

    int scale = tab->state->eximg.scaleValue;

    if (scale != SCALE_1x) {
        float scaleValue = 1.0f;
        if (scale == SCALE_05x) {
            scaleValue = 0.5f;
        } else if (scale == SCALE_025x) {
            scaleValue = 0.25f;
        } else if (scale != SCALE_1x && scale >= SCALE_2x &&
                   scale <= SCALE_10x) {
            scaleValue = scale - 1;
        }
        int newWidth = ceilf((float)tab->canvasWidth * scaleValue);
        int newHeight = ceilf((float)tab->canvasHeight * scaleValue);

        ImageResizeNN(&img, newWidth, newHeight);
    }

    return img;
}

bool TabExportImage(TabObj *tab) {
    const char *filename = tab->state->eximg.exportPath;
    if (TextLength(filename) <= 0) {
        TraceLog(LOG_ERROR, "Invalid export filename");
        return false;
    }

    BpImgFmt imgfmt = getFormat(GetFileExtension(filename));

    if (imgfmt == BP_IMG_COUNT) {
        TraceLog(
            LOG_ERROR, "Unsupported export format `%s`",
            GetFileExtension(filename)
        );
        return false;
    }

    Image patchedImg = getPatchedImage(tab);
    ExportImage(patchedImg, filename);
    UnloadImage(patchedImg);

    return true;
}

static double fdl = 0.2;
void TabPlayAnimation(TabObj *tab, double dt) {
    tab->animtime += dt;
    int framecount = tab->curLayer->flist->count;
    if (tab->animtime >= fdl) {
        tab->animtime -= fdl;

        if (tab->curFrame == framecount - 1) {
            tab->curFrame = 0;
        } else {
            tab->curFrame++;
        }

        tab->state->lb.curFrame = tab->curFrame;
        tab->state->cvs.curFrame = tab->curFrame;
    }
}

void SyncTabData(
    TabObj *tab, MenuBarState *menu, StatusBarState *status, double dt
) {

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

    if (tab->state->lb.timelineState != TIMELINE_PLAY &&
        tab->state->lb.curFrame != tab->curFrame) {
        tab->curFrame = tab->state->lb.curFrame;
        tab->state->cvs.curFrame = tab->curFrame;
    }

    if (tab->state->lb.timelineState == TIMELINE_PLAY) {
        TabPlayAnimation(tab, dt);
    }

    if (CurrentColorChanged(&tab->state->cb)) {
        tab->state->cvs.current = tab->state->cb.currentColor;
    }

    if (tab->state->lb.selectedLayer->index != tab->curLayer->index) {
        tab->curLayer = tab->state->lb.selectedLayer;
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

    t->dttime = GetFrameTime();
    t->animtime = GetFrameTime();
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
    t->curFrame = 0;

    UpdateCanvasLayers(&t->state->cvs, t->layers, t->curLayer);
    t->state->lb.list = t->layers;
    t->state->lb.curLayer = t->curLayer;
    t->state->lb.selectedLayer = t->curLayer;
    t->index = 0;

    t->canvasWidth = w;
    t->canvasHeight = h;

    t->state->lb.gridSize = (Vector2){w, h};

    t->bg = BLANK;
    t->colorCount = 0;
    t->colors = NULL; // fill be handled via stb_ds

    t->filepath = NULL; // will be set later
    t->state->dtb.maxBrushSize = (int)fmaxf((float)w, (float)h);
    t->state->eximg.layerlist = t->layers;
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
