#include "../include/widgets/drawtoolbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/components.h"
#include "../include/drawtools.h"
#include "../include/exported/drawtoolicons.h"
#include "../include/utils.h"

#include <stdbool.h>
#include <stdlib.h>
#define OPTBAR_HEIGHT 40

static const ToolInfo DrawingTools[] = {
    {DT_PENCIL, ICON_PENCIL_BIG, ICON_PENCIL_BIG},
    {DT_ERASER, ICON_RUBBER, ICON_RUBBER},
    {DT_LINE, ICON_CROSSLINE, ICON_CROSSLINE},
    {DT_CIRCLE, ICON_BREAKPOINT_OFF, ICON_BREAKPOINT_OFF},
    {DT_CIRCLE_FILL, ICON_BREAKPOINT_ON, ICON_BREAKPOINT_ON},
    {DT_RECT, ICON_PLAYER_STOP, ICON_PLAYER_STOP},
    {DT_RECT_FILL, ICON_GRID_FILL, ICON_GRID_FILL},
    {DT_BUCKET, ICON_COLOR_BUCKET, ICON_COLOR_BUCKET},
    {DT_PAN, ICON_CURSOR_HAND, ICON_CURSOR_HAND}
};

static DToolInfo NewTool(DrawTool tool, const unsigned char *img, int size) {

    DToolInfo t = {.tool = tool, .img = LoadImageFromMemory(".png", img, size)};
    t.txt = LoadTextureFromImage(t.img);
    return t;
}

static void FreeTool(DToolInfo tool) {
    if (tool.img.data != NULL) {
        UnloadImage(tool.img);
    }

    UnloadTexture(tool.txt);
}

ToolBtnInfo NewToolBtnInfo() {
    ToolBtnInfo tb = {0};
    tb.pencilTool = NewTool(DT_PENCIL, PENCIL_PNG_DATA, PENCIL_PNG_SIZE);

    tb.eraserTool = NewTool(DT_ERASER, ERASER_PNG_DATA, ERASER_PNG_SIZE);

    tb.lineTool = NewTool(DT_LINE, LINE_PNG_DATA, LINE_PNG_SIZE);

    tb.circleTool = NewTool(DT_CIRCLE, CIRCLE_PNG_DATA, CIRCLE_PNG_SIZE);

    tb.circleFillTool =
        NewTool(DT_CIRCLE_FILL, CIRCLE_FILL_PNG_DATA, CIRCLE_FILL_PNG_SIZE);

    tb.rectTool = NewTool(DT_RECT, RECT_PNG_DATA, RECT_PNG_SIZE);

    tb.rectFillTool =
        NewTool(DT_RECT_FILL, RECT_FILL_PNG_DATA, RECT_FILL_PNG_SIZE);

    tb.bucketTool = NewTool(DT_BUCKET, BUCKET_PNG_DATA, BUCKET_PNG_SIZE);

    tb.panTool = NewTool(DT_PAN, PAN_PNG_DATA, PAN_PNG_SIZE);

    return tb;
}

DrawToolBarState NewDrawToolBar() {
    DrawToolBarState dtb = {0};
    dtb.prop = NewWidgetProp();
    dtb.currentTool = DT_PENCIL;
    dtb.anchor = (Vector2){0, 0};
    dtb.bottom = (Vector2){0, 0};
    dtb.toolbarPadding = (Vector2){0, 10};
    dtb.toolBtnMargin = (Vector2){10, 9};

    dtb.toolsRect = (Rectangle){0};
    dtb.optRect = (Rectangle){0};

    dtb.prop.bounds.width = DEFAULT_DTBAR_WIDTH;
    dtb.prop.bounds.height = 0;

    dtb.brushSize = 1.0;
    dtb.maxBrushSize = 1;
    dtb.brushShape = BSP_SQAURE;
    dtb.brushSizeEdit = false;
    dtb.font = GuiGetFont();

    dtb.tools = NewToolBtnInfo();
    dtb.sliderHover = false;

    return dtb;
}
void FreeDrawToolBar(DrawToolBarState *state) {
    if (state == NULL) {
        return;
    }

    FreeTool(state->tools.pencilTool);
    FreeTool(state->tools.eraserTool);
    FreeTool(state->tools.lineTool);
    FreeTool(state->tools.circleTool);
    FreeTool(state->tools.circleFillTool);
    FreeTool(state->tools.rectTool);
    FreeTool(state->tools.rectFillTool);
    FreeTool(state->tools.bucketTool);
    FreeTool(state->tools.panTool);
}

static void updateBounds(DrawToolBarState *dtb) {
    dtb->prop.bounds.x = dtb->anchor.x + DTBAR_MARGIN_L;
    dtb->prop.bounds.y = dtb->anchor.y;

    dtb->prop.bounds.width = DEFAULT_DTBAR_WIDTH;
    dtb->prop.bounds.height = GetScreenHeight() - dtb->bottom.y -
                              dtb->prop.bounds.y - DTBAR_MARGIN_TB;

    dtb->optRect =
        (Rectangle){dtb->prop.bounds.x, dtb->anchor.y + DTBAR_MARGIN_TB,
                    dtb->bottom.x - dtb->prop.bounds.x - DTBAR_MARGIN_L,
                    OPTBAR_HEIGHT};

    dtb->toolsRect =
        (Rectangle){dtb->prop.bounds.x,
                    dtb->prop.bounds.y + OPTBAR_HEIGHT + DTBAR_MARGIN_TB * 2,
                    dtb->prop.bounds.width, dtb->prop.bounds.height};
}

void SetDrawToolBarAnchor(
    DrawToolBarState *dtb, Vector2 anchor, Vector2 bottom
) {
    if (anchor.x != -1) {
        dtb->anchor.x = anchor.x;
    }

    if (anchor.y != -1) {
        dtb->anchor.y = anchor.y;
    }

    if (bottom.x != -1) {
        dtb->bottom.x = bottom.x;
    }

    if (bottom.y != -1) {
        dtb->bottom.y = bottom.y;
    }

    updateBounds(dtb);
}

GuiIconName getIconName(int b) {
    if (b == (int)DT_PENCIL) {
        return ICON_PENCIL;
    } else if (b == (int)DT_ERASER) {
        return ICON_RUBBER;
    } else if (b == (int)DT_LINE) {
        return ICON_WAVE_TRIANGULAR;
    } else if (b == (int)DT_PAN) {
        return ICON_CURSOR_HAND;
    }

    return ICON_BOX;
}

DrawTool HandleDToolsShortcuts(DrawToolBarState *state) {
    DrawTool tool = state->currentTool;

    if (GuiIsLocked()) {
        return tool;
    }

    if (IsKeyPressed(KEY_B)) {
        tool = DT_PENCIL;
    } else if (IsKeyPressed(KEY_E)) {
        tool = DT_ERASER;
    } else if (IsKeyPressed(KEY_L)) {
        tool = DT_LINE;
    } else if (IsKeyPressed(KEY_M)) {
        tool = DT_PAN;
    } else if (IsKeyPressed(KEY_C)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            tool = DT_CIRCLE_FILL;
        } else {
            tool = DT_CIRCLE;
        }
    } else if (IsKeyPressed(KEY_R)) {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            tool = DT_RECT_FILL;
        } else {
            tool = DT_RECT;
        }
    } else if (IsKeyPressed(KEY_G)) {
        tool = DT_BUCKET;
    }

    if (tool != state->currentTool) {
        state->currentTool = tool;
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && !GuiIsLocked()) {
        float wheel = GetMouseWheelMove();
        state->brushSize += wheel;

        if (state->brushSize <= 0) {
            state->brushSize = 1;
        }
    }

    return tool;
}

static void OptToolsPencil(DrawToolBarState *state, Rectangle bounds) {
    DrawTool current = state->currentTool;
    if (current == DT_PENCIL || current == DT_ERASER || current == DT_LINE ||
        current == DT_CIRCLE || current == DT_CIRCLE_FILL ||
        current == DT_RECT || current == DT_RECT_FILL) {
        float px = bounds.x;
        float py = bounds.y;

        Rectangle rect = {px, py, bounds.height, bounds.height};

        DToolInfo shapes[] = {
            state->tools.rectFillTool, state->tools.circleFillTool
        };
        BpBrushShapeButton(rect, &state->brushShape, shapes);

        rect.x += rect.width + 10;
        int brushTextSize = TextLength(TextFormat("%dpx", state->brushSize)) *
                            state->font.baseSize;
        rect.width = brushTextSize;
        BpInputSliderInt(
            rect, &state->brushSize, 0, state->maxBrushSize, "px",
            &state->brushSizeEdit, &state->sliderHover
        );
    }
}

static int DrawOptToolbar(DrawToolBarState *state) {
    Rectangle bounds = state->optRect;

    Rectangle drawBounds = {
        bounds.x + DTBAR_MARGIN_L, bounds.y + 5,
        bounds.width - DTBAR_MARGIN_L - DTBAR_MARGIN_R, bounds.height - 5 * 2
    };

    // BpRoundedPanel(bounds, 2, 0.125, true);
    BpPanelBorder(bounds, 2);
    OptToolsPencil(state, drawBounds);
    return -1;
}

static bool otherPen = false;
static bool otherEraser = false;
static bool otherLine = false;
static bool otherCircle = false;
static bool otherRect = false;
static bool otherBucket = false;
static bool otherPan = false;

#define rd (float)0.125

int DrawToolbar(DrawToolBarState *state) {
    if (state->prop.active) {

        updateBounds(state);

        Rectangle bounds = state->prop.bounds;
        Vector2 mpos = GetMousePosition();

        if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        DrawOptToolbar(state);

        Rectangle toolsRect = state->toolsRect;

        toolsRect.height = state->bottom.y - toolsRect.y - DTBAR_MARGIN_TB * 2;

        // BpRoundedPanel(toolsRect, 2, 0.125, true);
        BpPanelBorder(toolsRect, 2);

        float yInc = state->toolbarPadding.y + DEFAULT_DT_BTN_SIZE;
        Rectangle btnRect = {
            (toolsRect.x + toolsRect.width / 2.0f) -
                (DEFAULT_DT_BTN_SIZE / 2.0f),
            toolsRect.y + state->toolbarPadding.y,
            DEFAULT_DT_BTN_SIZE,
            DEFAULT_DT_BTN_SIZE,
        };

        ToolInfo pencilTool[] = {
            {DT_PENCIL, ICON_PENCIL_BIG, ICON_PENCIL_BIG},
            {DT_ERASER, ICON_RUBBER, ICON_RUBBER},
        };

        state->currentTool = BpDToolButton(
            btnRect, state->currentTool, &otherPen, ArrCount(pencilTool),
            pencilTool
        );

        /*
DToolInfo penTools[] = {state->tools.pencilTool};
DToolInfo eraserTools[] = {state->tools.eraserTool};
DToolInfo lineTools[] = {state->tools.lineTool};
DToolInfo rectTools[] = {
    state->tools.rectTool, state->tools.rectFillTool
};
DToolInfo circleTools[] = {
    state->tools.circleTool, state->tools.circleFillTool
};
DToolInfo bucketTools[] = {state->tools.bucketTool};

DToolInfo panTools[] = {state->tools.panTool};

state->currentTool = BpToolButton(
    btnRect, state->currentTool, &otherPen, ArrCount(penTools), penTools
);

btnRect.y += yInc;

state->currentTool = BpToolButton(
    btnRect, state->currentTool, &otherEraser, ArrCount(eraserTools),
    eraserTools
);

btnRect.y += yInc;

state->currentTool = BpToolButton(
    btnRect, state->currentTool, &otherLine, ArrCount(lineTools),
    lineTools
);

btnRect.y += yInc;

state->currentTool = BpToolButton(
    btnRect, state->currentTool, &otherCircle, ArrCount(rectTools),
    rectTools
);

btnRect.y += yInc;

state->currentTool = BpToolButton(
    btnRect, state->currentTool, &otherRect, ArrCount(circleTools),
    circleTools
);

btnRect.y += yInc;

state->currentTool = BpToolButton(
    btnRect, state->currentTool, &otherBucket, ArrCount(bucketTools),
    bucketTools
);

btnRect.y += yInc;

state->currentTool = BpToolButton(
    btnRect, state->currentTool, &otherPan, ArrCount(panTools), panTools
);
                */
    }
    return -1;
}
