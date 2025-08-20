#ifndef WIN_LAYEROPTS_H
#define WIN_LAYEROPTS_H

#include "../layers.h"
#include "window.h"
#include <stdbool.h>
typedef struct WLayerOptsState {
    WinProp p;
    LayerObj *layer;
    bool editName;
    char name[256];
    int opacityVal;
    bool opacityClicked;
} WLayerOptsState;

WLayerOptsState NewWLayerOpts();
WinStatus WLayerOpts(WLayerOptsState *state);
void FreeWLayerOpts(WLayerOptsState *state);

#endif
