#ifndef WIN_EXPORTIMG_H
#define WIN_EXPORTIMG_H

#include "../layers.h"
#include "window.h"
#include <stdbool.h>

typedef struct WExportImgState {
    WinProp p;

    bool editExportPath;
    char exportPath[512];
    char exportFilename[256];

    bool editScale;
    int scaleValue;

    bool editExpoLayer;
    int expoLayerValue;

    const LayerList *layerlist;
    char *layerNames;

} WExportImgState;

WExportImgState NewWExportImage();
void SetupWExportImage(WExportImgState *state);
WinStatus WExportImg(WExportImgState *state, double dt);
void FreeWExportImg(WExportImgState *state);

#endif
