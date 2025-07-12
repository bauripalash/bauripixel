// #define DEBUG

#include "external/raylib.h"
#include "external/raymath.h"
#include "include/colors.h"
#include "include/themes/theme.h"
#include "include/utils.h"
#include "include/widgets/canvas.h"
#include "include/widgets/colorbar.h"
#include "include/widgets/drawtoolbar.h"
#include "include/widgets/widget.h"
#include <math.h>
#include <stdbool.h>

// #define RAYGUI_GRID_ALPHA 1.0f
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"
#define STB_DS_IMPLEMENTATION
#include "external/stb/stb_ds.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 700

static int i = 0;
static int oldHCb = 0;
static int oldVCb = 0;

ColorBarState cb;
CanvasState canvas;
DrawToolBarState dtb;

void Layout();
void ApplyStyle();

#define DISABLE_COLORBAR

int RunApp() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BauriPixel");
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(60);
    LoadAppLightTheme();

    cb = NewColorBar();
    for (int i = 0; i < 8; i++) {
        AddToColorBar(&cb, ColorRedDark);
        AddToColorBar(&cb, ColorGreenDarkest);
        AddToColorBar(&cb, ColorGreenLighter);
        AddToColorBar(&cb, ColorGreenLightest);
        AddToColorBar(&cb, ColorBlueDarkest);
        AddToColorBar(&cb, ColorBlueLighter);
        AddToColorBar(&cb, ColorBlueLightest);
    }

    dtb = NewDrawToolBar();
    cb.prop.active = true;

    canvas = NewCanvas();
    canvas.prop.active = true;

    SetColorBarAnchor(&cb, (Vector2){-1, 50}, Vector2Zero());

    SetCanvasAnchor(
        &canvas, (Vector2){dtb.prop.bounds.width, 50},
        (Vector2){cb.prop.bounds.width, 50}
    );

    dtb.anchor.x = 0;
    dtb.anchor.y = 50 + CANVAS_MARGIN_TB;
    oldHCb = cb.prop.bounds.width;
    oldVCb = cb.prop.bounds.height;

    CenterAlignCanvas(&canvas);

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 5);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(ColorGrayDarkest);
            Layout();
        }
        EndDrawing();
    }

    ExportImage(canvas.canvasImg, "_temp.png");
    ClearColorBar(&cb);
    CloseWindow();

    return 0;
}

float cw = 100.0f;
float handleT = 20.0f;
float cboxSize = 30.0f;

int colorCount = 10;

bool handlDragging = false;
Vector2 scroll = {0, 0};

void cBar() {
    colorCount = cb.colorCount;
    Vector2 mpos = GetMousePosition();
    float canvasEnd = canvas.prop.bounds.x + canvas.prop.bounds.width;
    Rectangle rect = {GetScreenWidth() - cw, 50 + CANVAS_MARGIN_TB, cw, 200};

    Rectangle handleRect = {
        rect.x - (handleT / 2.0f), rect.y + handleT, handleT,
        rect.height - handleT * 2
    };

    bool atRect = CheckCollisionPointRec(mpos, rect);
    bool atHandle = CheckCollisionPointRec(mpos, handleRect);

    if (atHandle && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        handlDragging = true;
    }

    if (handlDragging) {
        Vector2 delta = GetMouseDelta();
        cw -= delta.x;
    }

    cw = Clamp(cw, 100.0f, 200.0f);

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        handlDragging = false;
    }

    DrawRectangleLinesEx(rect, 2, ColorGrayLightest);
    DrawRectangleRec(
        handleRect, Fade(ColorBlueLightest, atHandle ? 0.5f : 0.2f)
    );

    float halfBox = cboxSize / 2.0f;
    Rectangle uRect = {
        rect.x + halfBox,
        rect.y + halfBox,
        rect.width - (halfBox * 2),
        rect.height - (halfBox * 2),
    };

    int columnMax = (int)floorf(uRect.width / cboxSize);
    int rowMax = (int)floorf(uRect.height / cboxSize);

    int rowsUsed = colorCount / columnMax;
    int columnUsed = (colorCount < columnMax) ? colorCount : columnMax;
    int lastRowColum = colorCount % columnMax;

    if (lastRowColum > 0) {
        rowsUsed++;
    }

    Rectangle usedRect = {
        uRect.x,
        uRect.y,
        columnUsed * cboxSize,
        rowsUsed * cboxSize,
    };

    BeginScissorMode(uRect.x, uRect.y, uRect.width, uRect.height);

    for (int b = 0; b < colorCount; b++) {
        int col = b % columnMax;
        int row = b / columnMax;
        Rectangle bRect = {
            uRect.x + scroll.x + (col * cboxSize),
            uRect.y + scroll.y + (row * cboxSize), cboxSize, cboxSize
        };

        DrawRectangleRec(bRect, cb.colors[b]);
    }

    // DrawRectangleLinesEx(usedRect, 2, MAROON);

    EndScissorMode();

    int ogDefBG = GuiGetStyle(DEFAULT, BACKGROUND_COLOR);
    int ogBorderWidth = GuiGetStyle(LISTVIEW, BORDER_WIDTH);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, HexColorTransparent);
    GuiSetStyle(LISTVIEW, BORDER_WIDTH, 0);

    GuiScrollPanel(uRect, NULL, usedRect, &scroll, NULL);

    GuiSetStyle(LISTVIEW, BORDER_WIDTH, ogBorderWidth);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ogDefBG);

    TraceLog(
        LOG_ERROR, "USED [%d %d %d] | MAX [%d %d]", rowsUsed, columnUsed,
        lastRowColum, rowMax, columnMax
    );
}

void Layout() {
    DrawRectangleLinesEx(
        (Rectangle){-2, 0, GetScreenWidth() + 4, 22}, 2, ColorGrayLightest
    );
    DrawText("File", 10, 5, 10, ColorWhite);
    DrawText("Edit", 40, 5, 10, ColorWhite);
    DrawText("View", 70, 5, 10, ColorWhite);
    DrawText("Help", 100, 5, 10, ColorWhite);
    Color clr = cb.currentColor;
    Color txtClr = clr;
    if (ColorIsEqual(txtClr, ColorGrayDarkest)) {
        txtClr = WHITE;
    }
    // DrawTextEx(
    //     GetFontDefault(),
    //     TextFormat("C [%d, %d, %d, %d]", clr.r, clr.g, clr.b, clr.a),
    //     (Vector2){10, 10}, 20, 1, txtClr
    //);
    DrawToolbar(&dtb);
    canvas.curTool = dtb.currentTool;
    // canvas.current = cb.currentColor;
    Canvas(&canvas);
    UpdateCanvasAnchor(
        &canvas, (Vector2){-1, -1},
        (Vector2){cb.prop.bounds.width + CBAR_MARGIN_LEFT, -1}
    );
    // cBar();
    ColorBar(&cb);

    i++;
}
