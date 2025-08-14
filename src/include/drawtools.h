#ifndef DRAWTOOLS_H
#define DRAWTOOLS_H
#include "../external/raygui.h"

// Draw Tools
typedef enum DrawTool {
    DT_NOTOOL = -1,
    DT_PENCIL = 0,
    DT_ERASER,
    // Draw Line
    DT_LINE,

    DT_CIRCLE,
    DT_CIRCLE_FILL,
    DT_RECT,
    DT_RECT_FILL,
    DT_BUCKET,

    DT_PAN

} DrawTool;

typedef enum BrushShape {
    BSP_SQAURE,
    BSP_CIRCLE,

    // Custom Brush {NOT YET}
    BSP_CUSTOM,
} BrushShape;

typedef struct ToolInfo {
    DrawTool tool;
    GuiIconName iconTop;
    GuiIconName iconBottom;
} ToolInfo;

#endif
