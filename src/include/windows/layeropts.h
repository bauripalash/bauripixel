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
    float ogOpacity;
    char ogName[256];
} WLayerOptsState;

WLayerOptsState NewWLayerOpts();
void SetupWLayerOpts(WLayerOptsState *state, LayerObj *layer);
WinStatus WLayerOpts(WLayerOptsState *state, double dt);
void FreeWLayerOpts(WLayerOptsState *state);

#endif
