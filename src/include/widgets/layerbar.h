#ifndef LAYERBAR_H
#define LAYERBAR_H

#include "../../external/raylib.h"
#include "../layers.h"
#include "../windows/layeropts.h"
#include "widget.h"
#include <stdbool.h>

// Timeline animation playing state
typedef enum TimeLineState {
	// Play the animation
    TIMELINE_PLAY,
	// Stop the animation
    TIMELINE_STOP,
	// Pause the animation
    TIMELINE_PAUSE,
} TimeLineState;

// Layerbar widget state
typedef struct LayerBarState {
	// Widget prop base
    WidgetProp p;
	// Widget top left anchor
    Vector2 anchor;
	// Widget bottom right anchor
    Vector2 bottom;
    // Should the layer preview be shown
    bool preview;

    // Usable area (excludes margins and stuff)
    Rectangle usableRect;
    // Where the the toolbar of layerbar will be
    Rectangle toolsRect;
    // Where the layer list will be drawn
    Rectangle layersRect;

    // Where frames will be drawn; used for scrollbars
    Rectangle framesRect;

	// Vertical scrollbar bounds
    Rectangle vScrollRect;
	// Horizontal scrollbar bounds
    Rectangle hScrollRect;
	// Is vertical scroll thumb being dragged/used
    bool vScrollDragging;
	// Is horizontal scroll thumb being dragged/used
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

	// Current frame index
    int curFrame;

    // Background image from canvas widget
    Texture2D *previewBg;

    // Used for layer list scrolling
    Vector2 scroll;
    // Used layerlist view for scissor view
    Rectangle view;

	// Is the resize handle being dragged
    bool resizeDragging;

    // Layer options state
    WLayerOptsState wLayerOpts;

    // If any popup is active
    bool anypopup;

    // Right click context selected layer from menu
    LayerObj *menuSelLayer;
	// Is the context menu open
    bool menuOpen;
	// Context menu opening position, where the mouse was when right clicked 
    Vector2 menuPos;

    // Is a layer being dragged
    bool draggingLayer;
    // The layer being dragged
    LayerObj *dragLayer;

    // The layer index where the dragged layer is going to be
    int dragTarget;
    // Will the dragged layer be put below the `dragTarget` indexed layer
    bool putDragAtEnd;

	// layer name label width
    float layerNameWidth;

	// Timeline status, Play, Pause, Stop
    TimeLineState timelineState;

} LayerBarState;

// Create new layerbar state
LayerBarState NewLayerBar();
// Free the layerbar state
void FreeLayerBar(LayerBarState *lb);
// Set layerbar anchor
void SetLayerBarAnchor(LayerBarState *lb, Vector2 anchor, Vector2 bottom);
// Layerbar logic
int LayerBarLogic(LayerBarState *lb, double dt);
// Layerbar draw
int LayerBarDraw(LayerBarState *lb, double dt);

#endif // LAYERBAR_H
