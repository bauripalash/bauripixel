#ifndef TABS_H
#define TABS_H

#include "../external/raylib.h"
#include "layers.h"
#include "widgets/canvas.h"
#include "widgets/colorbar.h"
#include "widgets/drawtoolbar.h"

typedef struct TabStateObj {

    // Canvas
    CanvasState cvs;

    // ColorBar
    ColorBarState cb;

    // DrawToolBar
    DrawToolBarState dtb;

} TabStateObj;

TabStateObj *NewTabState(int w, int h);
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
    char *filepath;

    TabStateObj *state;
} TabObj;

TabObj *NewTabObj(int w, int h);
void FreeTabObj(TabObj *tab);
void SyncTabData(TabObj *tab);
void AddColorToTab(TabObj *tab, Color color);

typedef struct TabList {
    TabObj **tabs;
    int count;
} TabList;

TabList *NewTabList();
void FreeTabList(TabList *list);
void AddToTabList(TabList *list, TabObj *tab);
void RemoveIdxTabList(TabList *list, int index);

#endif // TABS_H
