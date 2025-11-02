#ifndef CANVAS_DRAW_H
#define CANVAS_DRAW_H
#include "../../external/raylib.h"
#include "../widgets/canvas.h"

// Bresenham's line algorithm (modified) (prototype)
void BPDrawLine(
    CanvasState *state, Image *img, Vector2 a, Vector2 b, Color clr
);

// Canvas Draw to Screen
// `bounds` = Canvas drawing rectangle
void DrawingCanvasDraw(CanvasState *state, Rectangle bounds);

// Canvas Drawing Logic
// `bounds` = Canvas drawing rectangle
void DrawingCanvasLogic(CanvasState *state, Rectangle bounds);

#endif
