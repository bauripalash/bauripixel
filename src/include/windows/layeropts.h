#ifndef WIN_LAYEROPTS_H
#define WIN_LAYEROPTS_H

#include "../layers.h"
#include "window.h"
#include <stdbool.h>

// Layer Options Window State
typedef struct WLayerOptsState {

	// Base window prop
    WinProp p;
	// Layer object which the options are being shown
    LayerObj *layer;
	// Is the layer name being edited
    bool editName;
	// Layer name
    char name[256];
	// Layer opacity 
    int opacityVal;
	// Is the opacity being clicked
    bool opacityClicked;
	// Original opacity of the layer
    float ogOpacity;
	// Original name of the layer
    char ogName[256];
} WLayerOptsState;

// Create New Layer Options window state
WLayerOptsState NewWLayerOpts();
// Setup layer options window
void SetupWLayerOpts(WLayerOptsState *state, LayerObj *layer);
// Draw Layer options window
WinStatus WLayerOpts(WLayerOptsState *state, double dt);
// Free layer options window state
void FreeWLayerOpts(WLayerOptsState *state);

#endif
