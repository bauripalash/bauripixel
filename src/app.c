// #define DEBUG

#include <math.h>

#include <stdbool.h>

#include "include/colors.h"
#include "include/theme.h"
#include "include/widgets/canvas.h"
#include "include/widgets/colorbar.h"
#include "include/widgets/drawtoolbar.h"
#include "include/widgets/statusbar.h"
#include "include/widgets/widget.h"

#define STB_DS_IMPLEMENTATION
#include "external/stb/stb_ds.h"

#include "external/raylib.h"
#include "external/raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 700

static int i = 0;
static int oldHCb = 0;
static int oldVCb = 0;

ColorBarState cb;
CanvasState canvas;
DrawToolBarState dtb;
StatusBarState sb;

void Layout();
void ApplyStyle();

#define DISABLE_COLORBAR
#define TOP_SPACE 80

int RunApp() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(
        FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED | FLAG_MSAA_4X_HINT
    );
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BauriPixel");

    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(240);
    // LoadAppLightTheme();
    LoadAppDarkTheme();
    LoadAppFont();

    float sbarHeight = 30;
    sb = NewStatusBar();
    SetStatusBarPosition(&sb, 0, sbarHeight);

    cb = NewColorBar();
    AddToColorBar(&cb, ColorBlack);
    AddToColorBar(&cb, ColorPurple);
    AddToColorBar(&cb, ColorRedDark);
    AddToColorBar(&cb, ColorOrange);
    AddToColorBar(&cb, ColorYellow);
    AddToColorBar(&cb, ColorGreenLightest);
    AddToColorBar(&cb, ColorGreenLighter);
    AddToColorBar(&cb, ColorGreenDarkest);

    AddToColorBar(&cb, ColorBlueDarkest);
    AddToColorBar(&cb, ColorBlueLighter);
    AddToColorBar(&cb, ColorBlueLightest);
    AddToColorBar(&cb, ColorCyan);

    AddToColorBar(&cb, ColorWhite);
    AddToColorBar(&cb, ColorGrayLightest);
    AddToColorBar(&cb, ColorGrayLighter);
    AddToColorBar(&cb, ColorGrayDarkest);
    AddToColorBar(&cb, RAYWHITE);

    dtb = NewDrawToolBar();
    cb.prop.active = true;

    canvas = NewCanvas();
    canvas.prop.active = true;

    SetColorBarAnchor(&cb, (Vector2){-1, TOP_SPACE}, Vector2Zero());

    SetCanvasAnchor(
        &canvas, (Vector2){dtb.prop.bounds.width, TOP_SPACE},
        (Vector2){cb.prop.bounds.width, 30}
    );

    dtb.anchor.x = 0;
    dtb.anchor.y = TOP_SPACE + CANVAS_MARGIN_TB;
    dtb.optAnchor.y = 20;
    oldHCb = cb.prop.bounds.width;
    oldVCb = cb.prop.bounds.height;

    sb.canvas = &canvas;
    sb.colorbar = &cb;
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

void Layout() {
    DrawRectangleLinesEx(
        (Rectangle){-2, 0, GetScreenWidth() + 4, 22}, 2, ColorGrayLightest
    );
    Font fnt = GuiGetFont();
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
    canvas.curTool = dtb.currentTool;
    canvas.brushSize = (int)floorf(dtb.brushSize);
    canvas.brushShape = dtb.brushShape;
    // canvas.current = cb.currentColor;
    Canvas(&canvas);
    UpdateCanvasAnchor(
        &canvas, (Vector2){-1, -1},
        (Vector2){cb.prop.bounds.width + CBAR_MARGIN_LEFT, -1}
    );
    // cBar();
    ColorBar(&cb);

    if (CurrentColorChanged(&cb)) {
        canvas.current = cb.currentColor;
    }

    DrawToolbar(&dtb);
    // DrawFPS(200, 0);

    // DrawTextEx(GuiGetFont(), "BauriPixel", (Vector2){100, 50}, 72, 0, WHITE);

    StatusBar(&sb);
    i++;
}
