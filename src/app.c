// #define DEBUG

#include "external/raylib.h"
#include "include/colors.h"
#include "include/themes/theme.h"
#include "include/utils.h"
#include "include/widgets/canvas.h"
#include "include/widgets/colorbar.h"
#include "include/widgets/widget.h"
#include <stdbool.h>

// #define RAYGUI_GRID_ALPHA 1.0f
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"
#define STB_DS_IMPLEMENTATION
#include "external/stb/stb_ds.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 700
int WinWidth = WINDOW_WIDTH;
int WinHeight = WINDOW_HEIGHT;
Vector2 mousePos = {0, 0};
#define GridCol 16
#define GridRow 16
int CellSize = 40;

static int i = 0;
static int oldHCb = 0;
static int oldVCb = 0;

ColorBarState cb;
CanvasState canvas;

void Layout();
void ApplyStyle();

int RunApp() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BauriPixel");
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(60);
    LoadAppLightTheme();

    cb = NewColorBar();
    for (int i = 0; i < 8; i++) {
        AddToColorBar(&cb, RED);
        AddToColorBar(&cb, GREEN);
        AddToColorBar(&cb, BLUE);
        AddToColorBar(&cb, PINK);
        AddToColorBar(&cb, MAGENTA);
    }
    SetColorBarAnchor(&cb, (Vector2){0, 50}, (Vector2){-1, 50});
    cb.prop.active = true;

    canvas = NewCanvas();
    canvas.prop.active = true;
    SetCanvasAnchor(
        &canvas, (Vector2){cb.anchor.x + cb.prop.bounds.width, 50},
        (Vector2){30, 50}
    );

    oldHCb = cb.prop.bounds.width;
    oldVCb = cb.prop.bounds.height;

    CenterAlignCanvas(&canvas);

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 5);

    while (!WindowShouldClose()) {
        WinWidth = GetScreenWidth();
        WinHeight = GetScreenHeight();
        mousePos = GetMousePosition();

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

Color currentColor = MAGENTA;
Color prevColor = RAYWHITE;

void Layout() {
    Color clr = cb.currentColor;
    Color txtClr = clr;
    if (ColorIsEqual(txtClr, ColorGrayDarkest)) {
        txtClr = WHITE;
    }
    DrawTextEx(
        GetFontDefault(),
        TextFormat("C [%d, %d, %d, %d]", clr.r, clr.g, clr.b, clr.a),
        (Vector2){10, 10}, 20, 1, txtClr
    );
    Canvas(&canvas);

    ColorBar(&cb);

    SetCurrentCanvasColor(&canvas, cb.currentColor);
    float d = cb.prop.bounds.width;

    if (oldHCb != cb.prop.bounds.width) {
        UpdateCanvasAnchor(
            &canvas, (Vector2){cb.prop.bounds.width, 50}, (Vector2){0, 50}
        );
    }

    oldHCb = cb.prop.bounds.width;

    i++;
}
