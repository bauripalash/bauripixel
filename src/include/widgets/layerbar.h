#ifndef LAYERBAR_H
#define LAYERBAR_H

#include "../../external/raylib.h"
#include "../layers.h"
#include "../windows/layeropts.h"
#include "widget.h"
#include <stdbool.h>

typedef struct LayerBarState {
    WidgetProp p;
    Vector2 anchor;
    Vector2 bottom;
    bool preview;

    Rectangle usableRect;
    Rectangle toolsRect;
    Rectangle layersRect;

    Rectangle vScrollRect;
    Rectangle hScrollRect;
    bool vScrollDragging;
    bool hScrollDragging;

    Vector2 gridSize;
    LayerList *list;
    LayerObj *curLayer;
    LayerObj *selLayer;
    Texture2D *previewBg;

    Vector2 scroll;
    Rectangle view;

    bool resizeDragging;
    WLayerOptsState wLayerOpts;
    bool anypopup;

} LayerBarState;

LayerBarState NewLayerBar();
void FreeLayerBar(LayerBarState *lb);
void SetLayerBarAnchor(LayerBarState *lb, Vector2 anchor, Vector2 bottom);
int LayerBarLogic(LayerBarState *lb);
int LayerBarDraw(LayerBarState *lb);

#endif // LAYERBAR_H
