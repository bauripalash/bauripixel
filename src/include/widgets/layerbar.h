#ifndef LAYERBAR_H
#define LAYERBAR_H

#include "../../external/raylib.h"
#include "../layers.h"
#include "../windows/layeropts.h"
#include "widget.h"
#include <stdbool.h>

typedef enum TimeLineState {
    TIMELINE_PLAY,
    TIMELINE_STOP,
    TIMELINE_PAUSE,
} TimeLineState;

typedef struct LayerBarState {
    WidgetProp p;
    Vector2 anchor;
    Vector2 bottom;
    // Should the layer preview
    bool preview;

    // Usable area (excludes margins and stuff)
    Rectangle usableRect;
    // Where the the toolbar of layerbar will be
    Rectangle toolsRect;
    // Where the layer list will be drawn
    Rectangle layersRect;

    // Where frames will be drawn; used for scrollbars
    Rectangle framesRect;

    Rectangle vScrollRect;
    Rectangle hScrollRect;
    bool vScrollDragging;
    bool hScrollDragging;

    // Current gridsize of project (canvas)
    Vector2 gridSize;
    // Layer list (comes from tab)
    LayerList *list;
    // Current layer (comes from Tab object)
    // Incoming <<
    LayerObj *curLayer;

    // Selected layer (is used by tab to set current layer)
    // Outgoing >>
    LayerObj *selectedLayer;

    int curFrame;
    bool selAllFrames;

    // Background image from canvas widget
    Texture2D *previewBg;

    // Used from layer list scrolling
    Vector2 scroll;
    // Used layerlist view for scissor view
    Rectangle view;

    bool resizeDragging;

    // Layer options state
    WLayerOptsState wLayerOpts;

    // If any popup is active
    bool anypopup;

    // Right click selected layer from menu
    LayerObj *menuSelLayer;
    bool menuOpen;
    Vector2 menuPos;

    // Is a layer is being dragged
    bool draggingLayer;
    // The layer being dragged
    LayerObj *dragLayer;

    // The layer index where the dragged layer is going to be
    int dragTarget;
    // Will the dragged layer be put below the `dragTarget` indexed layer
    bool putDragAtEnd;

    float layerNameWidth;

    TimeLineState timelineState;

} LayerBarState;

LayerBarState NewLayerBar();
void FreeLayerBar(LayerBarState *lb);
void SetLayerBarAnchor(LayerBarState *lb, Vector2 anchor, Vector2 bottom);
int LayerBarLogic(LayerBarState *lb);
int LayerBarDraw(LayerBarState *lb);

#endif // LAYERBAR_H
