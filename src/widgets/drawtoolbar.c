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
    dtb.brushShape = BSP_SQAURE;
    dtb.brushSizeEdit = false;

    dtb.tools = NewToolBtnInfo();

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

#define XBTN_SZ 50

static bool
DtButton(Rectangle bounds, const char *text, GuiIconName icon, bool isActive) {
    float bx = bounds.x;
    float by = bounds.y;
    float bw = bounds.width;
    float bh = bounds.height;
    Color fg = ColorGrayDarkest;
    int iconScale = (int)floorf((float)DEFAULT_DT_BTN_SIZE / 16.0f);
    int iconSize = iconScale * 16;
    bool clicked = CheckCollisionPointRec(GetMousePosition(), bounds) &&
                   IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if (clicked || isActive) {
        by += 3;
        fg = Fade(ColorGrayLighter, 0.5);
    } else {
        DrawRectangleRounded(
            (Rectangle){bx, by, bw, bh + 3}, 0.2, 0, ColorBlack
        );
    }

    Rectangle bRect = {bx, by, bw, bh};
    DrawRectangleRounded(bRect, 0.2, 0, fg);
    DrawRectangleRoundedLinesEx(bRect, 0.2, 0, 2, ColorBlack);
    DrawRectangleRoundedLinesEx(
        (Rectangle){
            bx + 2,
            by + 2,
            bw - 4,
            bh - 4,
        },
        0.2, 0, 2, ColorGrayLightest
    );
    GuiDrawIcon(
        icon, (bx) + (bw / 2.0f) - (iconSize / 2.0f),
        (by) + (bh / 2.0f) - (iconSize / 2.0f), iconScale, ColorWhite
    );

    return clicked;
}

static DrawTool handleShortcuts(DrawToolBarState *state) {
    DrawTool tool = state->currentTool;

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
        rect.width = 60;

        BpInputSliderInt(
            rect, &state->brushSize, 0, 32, "px", &state->brushSizeEdit
        );

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
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

    DrawRectangleRounded(bounds, 0.125, 0, ColorFDGrayLighter);

    DrawRectangleRoundedLinesEx(bounds, 0.125, 0, 3, ColorBlack);
    DrawRectangleRoundedLinesEx(
        (Rectangle){bounds.x + 2, bounds.y + 2, bounds.width - 4,
                    bounds.height - 4},
        0.125, 0, 2, ColorGrayLightest
    );

    OptToolsPencil(state, drawBounds);
    return -1;
}

static bool otherPen = false;
static bool otherEraser = false;
static bool otherLine = false;
static bool otherCircle = false;
static bool otherRect = false;
static bool otherPan = false;

int DrawToolbar(DrawToolBarState *state) {
    updateBounds(state);
    handleShortcuts(state);

    Rectangle bounds = state->prop.bounds;
    Vector2 mpos = GetMousePosition();

    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    DrawRectangleRounded(
        (Rectangle){state->prop.bounds.x, state->prop.bounds.y,
                    state->prop.bounds.width, state->prop.bounds.height},
        0.125, 0, ColorFDGrayLighter
    );

    DrawRectangleRoundedLinesEx(bounds, 0.125, 0, 3, ColorBlack);
    DrawRectangleRoundedLinesEx(
        (Rectangle){bounds.x + 2, bounds.y + 2, bounds.width - 4,
                    bounds.height - 4},
        0.125, 0, 2, ColorGrayLightest
    );

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
