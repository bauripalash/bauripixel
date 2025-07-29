#include "../include/widgets/drawtoolbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/drawtools.h"
#include "../include/exported/drawtoolicons.h"
#include "../include/utils.h"

#include <math.h>
#include <stdbool.h>

static DToolInfo NewTool(DrawTool tool, const unsigned char *img, int size) {

    DToolInfo t = {.tool = tool, .img = LoadImageFromMemory(".png", img, size)};
    t.txt = LoadTextureFromImage(t.img);
    return t;
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

    tb.panTool = NewTool(DT_PAN, PAN_PNG_DATA, PAN_PNG_SIZE);

    return tb;
}

DrawToolBarState NewDrawToolBar() {
    DrawToolBarState dtb = {0};
    dtb.prop = NewWidgetProp();
    dtb.currentTool = DT_PENCIL;
    dtb.anchor = (Vector2){0, 0};
    dtb.toolbarPadding = (Vector2){0, 10};
    dtb.toolBtnMargin = (Vector2){10, 9};

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
void FreeDrawToolBar(DrawToolBarState *state) { return; }

static void updateBounds(DrawToolBarState *dtb) {
    dtb->prop.bounds.x = dtb->anchor.x + DTBAR_MARGIN_L;
    dtb->prop.bounds.y = dtb->anchor.y;

    dtb->prop.bounds.width = DEFAULT_DTBAR_WIDTH;
    dtb->prop.bounds.height = GetScreenHeight() - dtb->prop.bounds.y - 30 - 10;
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

static DrawTool handleShortcuts(DrawToolBarState *state) {
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
    }

    if (tool != state->currentTool) {
        state->currentTool = tool;
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

        if (IsKeyDown(KEY_LEFT_CONTROL) && !GuiIsLocked()) {
            float wheel = GetMouseWheelMove();
            state->brushSize += wheel;

            if (state->brushSize <= 0) {
                state->brushSize = 1;
            }
        }
    }
}

static int DrawOptToolbar(DrawToolBarState *state) {
    Rectangle bounds = {
        state->prop.bounds.x, state->optAnchor.y + DTBAR_MARGIN_TB,
        GetScreenWidth() - DTBAR_MARGIN_R - DTBAR_MARGIN_L, 60 - DTBAR_MARGIN_TB
    };

    Rectangle drawBounds = {
        bounds.x + DTBAR_MARGIN_L, bounds.y + DTBAR_MARGIN_TB,
        bounds.width - DTBAR_MARGIN_L - DTBAR_MARGIN_R,
        bounds.height - DTBAR_MARGIN_TB * 2
    };

    BpRoundedPanel(bounds, 0.125);
    OptToolsPencil(state, drawBounds);
    return -1;
}

static bool otherPen = false;
static bool otherEraser = false;
static bool otherLine = false;
static bool otherCircle = false;
static bool otherRect = false;
static bool otherPan = false;

#define rd (float)0.125

int DrawToolbar(DrawToolBarState *state) {
    updateBounds(state);
    handleShortcuts(state);

    Rectangle bounds = state->prop.bounds;
    Vector2 mpos = GetMousePosition();

    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    BpRoundedPanel(bounds, 0.125);

    float yInc = state->toolbarPadding.y + DEFAULT_DT_BTN_SIZE;
    Rectangle btnRect = {
        (state->prop.bounds.x + state->prop.bounds.width / 2.0f) -
            (DEFAULT_DT_BTN_SIZE / 2.0f),
        state->prop.bounds.y + state->toolbarPadding.y,
        DEFAULT_DT_BTN_SIZE,
        DEFAULT_DT_BTN_SIZE,
    };

    DToolInfo penTools[] = {state->tools.pencilTool};
    DToolInfo eraserTools[] = {state->tools.eraserTool};
    DToolInfo lineTools[] = {state->tools.lineTool};
    DToolInfo rectTools[] = {state->tools.rectTool, state->tools.rectFillTool};
    DToolInfo circleTools[] = {
        state->tools.circleTool, state->tools.circleFillTool
    };

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
        btnRect, state->currentTool, &otherLine, ArrCount(lineTools), lineTools
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
        btnRect, state->currentTool, &otherPan, ArrCount(panTools), panTools
    );

    DrawOptToolbar(state);

    return -1;
}
