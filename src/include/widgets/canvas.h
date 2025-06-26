#ifndef CANVAS_H
#define CANVAS_H

#include "widget.h"
#include <stdbool.h>

#define CANVAS_MARGIN_TB   10
#define CANVAS_MARGIN_L    20
#define CANVAS_MARGIN_R    10
#define CANVAS_DRAW_MARGIN 20
#define DEFAULT_GRID_SIZE  32
#define INIT_CELL_SIZE     16

typedef struct {
    WidgetProp prop;
    int hoverX;
    int hoverY;
    Vector2 anchor;
    Vector2 bottomStop;
    Vector2 gridSize;

    Vector2 scroll;
    Color colors[DEFAULT_GRID_SIZE][DEFAULT_GRID_SIZE];
    float zoom;
    float pxSize;
    float zoomMin;
    float zoomMax;
    Rectangle content;
    Rectangle view;

} CanvasState;

CanvasState NewCanvas();
void SetCanvasAnchor(CanvasState *c, Vector2 anchor, Vector2 bottom);
bool Canvas(CanvasState *state);

#endif
