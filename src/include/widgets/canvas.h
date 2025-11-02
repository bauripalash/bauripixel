#ifndef CANVAS_H
#define CANVAS_H

#include "widget.h"

#include "../drawtools.h"
#include "../layers.h"
#include <stdbool.h>

// Canvas Widget margin for Top and Bottom
#define CANVAS_MARGIN_TB   10
// Canvas Widget margin for Left side
#define CANVAS_MARGIN_L    10
// Canvas Widget margin for right side
#define CANVAS_MARGIN_R    10
// Default sprite side : TODO: Remove
#define DEFAULT_GRID_SIZE  32

// Canvas State Object
typedef struct {
	// Widget prop base
    WidgetProp prop;
	// Hover position in canvas size area : X
    int hoverX;
	// Hover position in canvas size area : Y
    int hoverY;
	// Canvas/Sprite Size
    Vector2 gridSize;
	// Top Left : Anchor for Canvas Widget
    Vector2 anchor;
	// Bottom right: Anchor for Canvas Widget
    Vector2 bottomAnchor;

	// Current selected color
    Color current;
	// Minimum Zoom for the scrolling camera
    float zoomMin;
	// Maximum zoom for the scrolling camera
    float zoomMax;
	// If Canvas panning enabled
    bool enablePanning;
	// Is the canvas being panned
    bool panning;

	// Canvas scroll Vector
    Vector2 scroll;
	// Vertical scrollbar rectangle
    Rectangle vScrollRect;
	// Horizontal scrollbar rectangle
    Rectangle hScrollRect;
	// Is Vertical scrollbar thumb is being dragged/used
    bool vScrollDragging;
	// Is Horizontal scrollbar thumb is being dragged/used
    bool hScrollDragging;
	// Scrollbar thickness
	// Scrollbar width for vertical scrollbar
	// Scrollbar height for horizontal scrollbar
    float scrollThickness;

	// Scroll+Zoom camera
    Camera2D camera;
	// Fake character vector which is actually being controlled
    Vector2 point;
	// The drawing area
    Rectangle drawArea;

	// The background image for drawing canvas
    Image bgImg;
	// The background image texture for drawing canvas
    Texture bgTxt;
	// The color for background
    Color bgColor;

    LayerList *layers;
    LayerObj *curLayer;
    int curFrame;

	// Preview texture 
	// Where the drawing actions are drawn for preview
    Texture previewTxt;
	// Image holder for Preview texture
    Image previewImg;

	// Current drawing tool
    DrawTool curTool;

	// Is the mouse cursor on canvas
    bool hoverCanvas;

	// Brush size for drawing tools
    int brushSize;
	// Current brush shape for drawing tools
    BrushShape brushShape;
	// Is the brush being dragged
    bool brushDragging;
	// Last brush position for continuous strokes
    Vector2 lastBrushPos;

	// Is the eraser being dragged
    bool eraserDragging;
	// Last eraser position
    Vector2 lastEraserPos;

	// Position where the line is being dragged from
    Vector2 lineStart;
	// Is the line tool being used/dragged
    bool lineDragging;

	// Position where the circle is being dragged from
    Vector2 circleStart;
	// Is the circle tool being used/dragged
    bool circleDragging;

	// Position where the rectangle is being dragged from
    Vector2 rectStart;
	// Is the rectangle tool being used/dragged
    bool rectDragging;

	// Draw Area Position
	// (Top Left X, Top Left Y, Bottom Right X, Bottom Right Y)
	// X, Y == Top left X,Y position
	// Z, W = Bottom right X, Y position
    Vector4 drawArea4;
	// Canvas Area (Top Left X, Top Left Y, Bottom Right X, Bottom Right Y)
	
	// Sprite/Drawing canvas Position
	// (Top Left X, Top Left Y, Bottom Right X, Bottom Right Y)
	// X,Y = Top Left X,Y position
	// Z,W = Bottom Right X,Y position
    Vector4 canvasArea4;

    // Scroll area horizontal thumb
    Rectangle sbHThumbRect;
    bool hoverHThumb;
    // Scroll area vertical thumb
    Rectangle sbVThumbRect;
    bool hoverVThumb;

	// Guide Grid size (according to canvas size)
    Vector2 guideGridSize;
	// Offset of Guide grid from top left corner of canvas
    Vector2 guideGridOffset;
	// Line thickness of guide grid
    float guidGridThickness;
	// Color of guide grid
    Color guideGridColor;

} CanvasState;

// Create a new sprite canvas (State)
// `w` = Width of Sprite
// `h` = Height of Sprite
CanvasState NewCanvas(int w, int h);
// Free canvas state
void FreeCanvas(CanvasState *c);
// Syncs layers and current selected layers
void UpdateCanvasLayers(CanvasState *c, LayerList *list, LayerObj *current);
// Set Canvas Widget Anchors
void SetCanvasAnchor(CanvasState *c, Vector2 anchor, Vector2 bottom);
// Update Canvas Anchors; Called every frame
void UpdateCanvasAnchor(CanvasState *c, Vector2 anchor, Vector2 bottom);
// Put the drawing canvas at the center of the widget
void CenterAlignCanvas(CanvasState *state);
// Set Selected color
void SetCurrentCanvasColor(CanvasState *state, Color color);
// Canvas Widget Logic
bool CanvasLogic(CanvasState *state, double dt);
// Canvas widget drawing
bool CanvasDraw(CanvasState *state, double dt);

#endif
