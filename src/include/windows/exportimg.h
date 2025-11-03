#ifndef WIN_EXPORTIMG_H
#define WIN_EXPORTIMG_H

#include "../layers.h"
#include "window.h"
#include <stdbool.h>

// Simple Export Window State 
typedef struct WExportImgState {

	// Base window prop
    WinProp p;

	// Export path being edited
    bool editExportPath;
	// Full export path
    char exportPath[512];
	// Export file name
    char exportFilename[256];

	// Export scale being edited
    bool editScale;
	// Export scale value
    int scaleValue;

	// Export layers being edited
    bool editExpoLayer;
	// Selected layer to export
	// All Layers, Visible Layers, (Layer 1, Layer 2....)
    int expoLayerValue;

	// link to Layers lists 
    const LayerList *layerlist;
	// Layer names created on the fly
    char *layerNames;

} WExportImgState;

// Create new export image state
WExportImgState NewWExportImage();
// Setup export state widgets
// Create layers name list etc.
void SetupWExportImage(WExportImgState *state);
// Draw Export Image window
WinStatus WExportImg(WExportImgState *state, double dt);
// Free Export Image State
void FreeWExportImg(WExportImgState *state);

#endif
