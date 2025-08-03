#ifndef LAYERBAR_H
#define LAYERBAR_H

#include "../../external/raylib.h"
#include "../layers.h"
#include "widget.h"
#include <stdbool.h>

typedef struct LayerBarState {
    WidgetProp p;
    Vector2 anchor;
    Vector2 bottom;
    bool preview;
    Vector2 gridSize;
    LayerList *list;
    LayerObj *curLayer;
    LayerObj *selLayer;
    Texture2D *previewBg;

    Vector2 scroll;
    Rectangle view;

    bool resizeDragging;

} LayerBarState;

LayerBarState NewLayerBar();
void FreeLayerBar(LayerBarState *lb);
void SetLayerBarAnchor(LayerBarState *lb, Vector2 anchor, Vector2 bottom);
int LayerBarLogic(LayerBarState *lb);
int LayerBarDraw(LayerBarState *lb);

#endif // LAYERBAR_H
