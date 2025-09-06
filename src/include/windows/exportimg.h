#ifndef WIN_EXPORTIMG_H
#define WIN_EXPORTIMG_H

#include "window.h"
#include <stdbool.h>

typedef struct WExportImgState {
    WinProp p;
    bool editExportPath;
    char exportPath[512];
    char exportFilename[256];
    bool editResize;

} WExportImgState;

WExportImgState NewWExportImage();
void SetupWExportImage(WExportImgState *state);
WinStatus WExportImg(WExportImgState *state);
void FreeWExportImg();

#endif
