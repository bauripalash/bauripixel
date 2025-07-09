#ifndef CANVAS_H
#define CANVAS_H

#include "widget.h"
#include <stdbool.h>

#define CANVAS_MARGIN_TB   10
#define CANVAS_MARGIN_L    20
#define CANVAS_MARGIN_R    10
#define CANVAS_DRAW_MARGIN 0
#define DEFAULT_GRID_SIZE  32
#define INIT_CELL_SIZE     16

typedef struct {
    WidgetProp prop;
    int hoverX;
    int hoverY;
    Vector2 gridSize;
    Vector2 anchor;
    Vector2 bottomAnchor;

    Color colors[DEFAULT_GRID_SIZE][DEFAULT_GRID_SIZE];
    Color current;
    float pxSize;
    float zoomMin;
    float zoomMax;
    bool enablePanning;
    bool panning;

    Vector2 scroll;
    Rectangle vScrollRect;
    Rectangle hScrollRect;
    bool vScrollDragging;
    bool hScrollDragging;
    Color scrollBody;
    Color scrollThumb;
    float scrollThickness;

    Camera2D camera;
    Vector2 point;
    Rectangle drawArea;

    Image canvasImg;
    Texture canvasTxt;
    Vector2 txtPos;

} CanvasState;

CanvasState NewCanvas();
void SetCanvasAnchor(CanvasState *c, Vector2 anchor, Vector2 bottom);
void UpdateCanvasAnchor(CanvasState *c, Vector2 anchor, Vector2 bottom);
void CenterAlignCanvas(CanvasState *state);
void SetCurrentCanvasColor(CanvasState *state, Color color);
bool Canvas(CanvasState *state);

#endif
