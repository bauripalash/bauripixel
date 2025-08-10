#ifndef CANVAS_H
#define CANVAS_H

#include "widget.h"

#include "../drawtools.h"
#include "../layers.h"
#include <stdbool.h>

#define CANVAS_MARGIN_TB   10
#define CANVAS_MARGIN_L    5
#define CANVAS_MARGIN_R    5
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
    float scrollThickness;

    Camera2D camera;
    Vector2 point;
    Rectangle drawArea;

    Image bgImg;
    Texture bgTxt;
    Color bgColor;

    LayerList *layers;
    LayerObj *curLayer;

    Texture previewTxt;
    Image previewImg;
    // Vector2 txtPos;
    DrawTool curTool;

    // NOTE: must be changed :

    bool hoverCanvas;

    int brushSize;
    BrushShape brushShape;

    Vector2 lineStart;
    bool lineDragging;

    Vector2 circleStart;
    bool circleDragging;

    Vector2 rectStart;
    bool rectDragging;

    Vector4 drawArea4;
    Vector4 canvasArea4;

    // Scroll area horizontal thumb
    Rectangle sbHThumbRect;
    bool hoverHThumb;
    // Scroll area vertical thumb
    Rectangle sbVThumbRect;
    bool hoverVThumb;

} CanvasState;

CanvasState NewCanvas(int w, int h);
void FreeCanvas(CanvasState *c);
void UpdateCanvasLayers(CanvasState *c, LayerList *list, LayerObj *current);
void SetCanvasAnchor(CanvasState *c, Vector2 anchor, Vector2 bottom);
void UpdateCanvasAnchor(CanvasState *c, Vector2 anchor, Vector2 bottom);
void CenterAlignCanvas(CanvasState *state);
void SetCurrentCanvasColor(CanvasState *state, Color color);
bool CanvasLogic(CanvasState *state);
bool CanvasDraw(CanvasState *state);

#endif
