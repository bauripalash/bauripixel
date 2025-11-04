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

// Tab State Object
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

// Create New Tab State
// `panel` = Bounds for the whole tab
// `w` = Sprite Width
// `h` = Sprite Height
TabStateObj *NewTabState(Rectangle panel, int w, int h);
// Free Tab State
void FreeTabState(TabStateObj *state);

// Tab Object
typedef struct TabObj {
	// Tab Index
    int index;
	// Global Delta Time
    double dttime;
	// Animation Spcific Time
	// Similar a stopwatch for Animation playing
	// If the current frames time goes beyong animtime go to the next frame
    double animtime;
	// Canvas/Sprite Height
    int canvasHeight;
	// Canvas/Sprite Width
    int canvasWidth;
	// Sprite Background
	// Usually Transparent or Solid Color
    Color bg;
	// How many colors in color palette
    int colorCount;
	// Color palette
    Color *colors;
	// List of Layers
    LayerList *layers;
	// Current Layer Selected
    LayerObj *curLayer;
	// Current frame index
    int curFrame;
	// Full File path for the sprite opened
    char *filepath;
	// Whole usable area for the tab 
    Rectangle tabPanel;

	// Has the initial tab setup been done
	// Adds the default palette to color palette
	// Sets the drawing canvas at the center of Canvas widget
	// Activate the child states
    bool setupDone;

	// Tab State
	// Grouping for underlying tab specific widget and window states
    TabStateObj *state;
} TabObj;

// Create a New Tab Object
// `w`  = Sprite Width
// `h` = Sprite Height
// Returns => Newly created Tab object or NULL on allocation failure
TabObj *NewTabObj(int w, int h);
// Free tab Object
void FreeTabObj(TabObj *tab);

// Setup Tab for the First Time
// Adds the default palette to color palette
// Sets the drawing canvas at the center of Canvas widget
// Activate the child states
// `menu` = Link to GUI's menu bar
// `status` = Link to GUI's status bar
// `dt` = Delta Time
void SetupTabData(
    TabObj *tab, MenuBarState *menu, StatusBarState *status, double dt
);

// Sync Tab States and Data per frame
// `menu` = Link to GUI's menu bar
// `status` = Link to GUI's status bar
// `dt` = Delta Time
void SyncTabData(
    TabObj *tab, MenuBarState *menu, StatusBarState *status, double dt
);

// Add `color` to Color palette of the Tab
void AddColorToTab(TabObj *tab, Color color);

// Export Sprite as Simple Single Image
bool TabExportImage(TabObj *tab);

// Tab List
typedef struct TabList {
	// Array of tabs
    TabObj **tabs;
	// Count of tabs in array
    int count;
} TabList;

// Create New Tab List
TabList *NewTabList();
// Free The Tab List and all Tab object inside it
void FreeTabList(TabList *list);
// Add New tab to the end of the tab list
void AddToTabList(TabList *list, TabObj *tab);
// Remove tab at index `index` from list
void RemoveIdxTabList(TabList *list, int index);

#endif // TABS_H
