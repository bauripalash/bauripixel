#ifndef TABS_H
#define TABS_H

#include "../external/raylib.h"
#include "layers.h"
#include "widgets/canvas.h"
#include "widgets/colorbar.h"
#include "widgets/drawtoolbar.h"
#include "widgets/layerbar.h"
#include "widgets/menubar.h"
#include "widgets/statusbar.h"
#include "windows/exportimg.h"
#include <stdbool.h>

typedef struct TabStateObj {

    // Canvas
    CanvasState cvs;

    // ColorBar
    ColorBarState cb;

    // DrawToolBar
    DrawToolBarState dtb;

    // LayerBar
    LayerBarState lb;

    // Windows
    WExportImgState eximg;

} TabStateObj;

TabStateObj *NewTabState(Rectangle panel, int w, int h);
void FreeTabState(TabStateObj *state);

typedef struct TabObj {
    int index;
    int canvasHeight;
    int canvasWidth;
    Color bg;
    int colorCount;
    Color *colors;
    LayerList *layers;
    LayerObj *curLayer;
    int curFrame;
    char *filepath;
    Rectangle tabPanel;

    bool setupDone;

    TabStateObj *state;
} TabObj;

TabObj *NewTabObj(int w, int h);
void FreeTabObj(TabObj *tab);

void SetupTabData(TabObj *tab, MenuBarState *menu, StatusBarState *status);
void SyncTabData(TabObj *tab, MenuBarState *menu, StatusBarState *status);
void AddColorToTab(TabObj *tab, Color color);
bool TabExportImage(TabObj *tab);

typedef struct TabList {
    TabObj **tabs;
    int count;
} TabList;

TabList *NewTabList();
void FreeTabList(TabList *list);
void AddToTabList(TabList *list, TabObj *tab);
void RemoveIdxTabList(TabList *list, int index);

#endif // TABS_H
