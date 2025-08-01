// #define DEBUG

#include <math.h>

#include <stdbool.h>

#include "include/colors.h"
#include "include/menuinfo.h"
#include "include/theme.h"
#include "include/widgets/canvas.h"
#include "include/widgets/colorbar.h"
#include "include/widgets/drawtoolbar.h"
#include "include/widgets/menubar.h"
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

static int oldHCb = 0;
static int oldVCb = 0;

ColorBarState cb;
CanvasState canvas;
DrawToolBarState dtb;
StatusBarState sb;
MenuBarState mb;

void Layout();
void ApplyStyle();

void LayoutDraw();
void LayoutLogic();

#define DISABLE_COLORBAR
#define TOP_SPACE 80

int RunApp() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(
        FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE
    );
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BauriPixel");

    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(60);
    LoadAppDarkTheme();
    LoadAppFont();

    float sbarHeight = 30;
    mb = NewMenuBar();
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
    dtb.maxBrushSize = (int)fmaxf(canvas.gridSize.x, canvas.gridSize.y);

    sb.canvas = &canvas;
    sb.colorbar = &cb;
    CenterAlignCanvas(&canvas);

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 5);

    while (!WindowShouldClose()) {
        LayoutLogic();
        BeginDrawing();
        {
            ClearBackground(ColorXDarkGray1);
            // Layout();
            LayoutDraw();
        }
        EndDrawing();
    }

    // ExportImage(canvas.canvasImg, "_temp.png");
    ClearColorBar(&cb);
    CloseWindow();

    return 0;
}

static MenuAction maction = MACTION_COUNT;

void handleMenubar() {
    if (maction != MACTION_COUNT) {
        if (maction == MACTION_SAVE_FILE) {
            ExportImage(canvas.canvasImg, "_temp.png");
            TraceLog(LOG_WARNING, "[+] Saved file as `_temp.png`");
        }
    }
}

void LayoutLogic() {

    if (GuiIsLocked())
        GuiUnlock();

    handleMenubar();

    bool menuBarOpen = mb.menuOpen;
    bool sliderHover = dtb.sliderHover;
    if (menuBarOpen || sliderHover) {
        GuiLock();
    }
    ColorBarLogic(&cb);
    if (sliderHover) {
        GuiUnlock();
    }
    HandleDToolsShortcuts(&dtb);
    if (sliderHover) {
        GuiLock();
    }
    canvas.curTool = dtb.currentTool;
    canvas.brushSize = dtb.brushSize;
    canvas.brushShape = dtb.brushShape;
    UpdateCanvasAnchor(
        &canvas, (Vector2){-1, -1},
        (Vector2){cb.prop.bounds.width + CBAR_MARGIN_LEFT, -1}
    );
    if (CurrentColorChanged(&cb)) {
        canvas.current = cb.currentColor;
    }

    CanvasLogic(&canvas);
}

void LayoutDraw() {
    bool menuBarOpen = mb.menuOpen;
    bool sliderHover = dtb.sliderHover;
    if (menuBarOpen || sliderHover) {
        GuiLock();
    }
    CanvasDraw(&canvas);
    ColorBarDraw(&cb);
    StatusBar(&sb);

    if (sliderHover)
        GuiUnlock();

    DrawToolbar(&dtb);

    if (sliderHover)
        GuiLock();

    if (menuBarOpen)
        GuiUnlock();

    maction = MenuBar(&mb);

    if (menuBarOpen)
        GuiLock();

    DrawFPS(200, 50);
}
