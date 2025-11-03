#ifndef DRAWTOOLS_H
#define DRAWTOOLS_H
#include "../external/raygui.h"

// Draw Tools
typedef enum DrawTool {
	// No Tool Is being selected
    DT_NOTOOL = -1,
	// Pencil tool
    DT_PENCIL = 0,
	// Eraser tool
    DT_ERASER,
    // Draw Line Tool
    DT_LINE,

	// Circle Tool
    DT_CIRCLE,
	// Filled Circle Tool
    DT_CIRCLE_FILL,
	// Rectangle Tool
    DT_RECT,
	// Filled Rectangle Tool
    DT_RECT_FILL,
	// Bucket Fill Tool
    DT_BUCKET,
	// Pan/Move Canvas Tool
    DT_PAN

} DrawTool;

// Brush Shape
typedef enum BrushShape {
	// Square Brush
    BSP_SQAURE,
	// Circular Brush
    BSP_CIRCLE,

    // Custom Brush
	// Not Yet Implemented
    BSP_CUSTOM,
} BrushShape;

#endif
