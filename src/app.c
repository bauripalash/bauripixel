#include "include/colors.h"
#include "include/components.h"
#include "include/gui.h"
#include "include/menuinfo.h"
#include "include/options.h"
#include "include/tabs.h"
#include "include/theme.h"
#include "include/utils.h"
#include "include/widgets/drawtoolbar.h"
#include "include/widgets/layerbar.h"
#include <stdbool.h>

#define STB_DS_IMPLEMENTATION
#include "external/stb/stb_ds.h"

#include "external/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 700

void LayoutDraw(Gui *gui);
void LayoutLogic(Gui *gui);

int RunApp() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(
        FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE
    );
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BauriPixel");
    SetWindowState(FLAG_WINDOW_MAXIMIZED);

    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(60);
    LoadAppDarkTheme();
    LoadAppFont();

    Gui *gui = NewGui();

    if (gui == NULL) {
        TraceLog(LOG_ERROR, "Failed to create Gui Object : TODO");
        return -1;
    }

    TabObj *initTab = NewTabObj(32, 32); // for experimenting
                                         // TD : Handle Error;

    AddColorToTab(initTab, ColorVGray);
    AddColorToTab(initTab, ColorVGrayLight);
    AddColorToTab(initTab, ColorVGreen);
    AddColorToTab(initTab, ColorVWhite);

    AddToTabList(gui->tabList, initTab);
    gui->curTab = gui->tabList->tabs[0];

    gui->state->statusbar.colorbar = &gui->curTab->state->cb;
    gui->state->statusbar.canvas = &gui->curTab->state->cvs;

    float sbarHeight = 30;

    SetStatusBarPosition(&gui->state->statusbar, 0, sbarHeight);

    SyncTabData(gui->curTab, &gui->state->menubar, &gui->state->statusbar);

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 5);

    while (!WindowShouldClose()) {
        LayoutLogic(gui);
        BeginDrawing();
        {
            ClearBackground(GetColor(OptThemeGet(T_APP_BG)));
            LayoutDraw(gui);
        }
        EndDrawing();
    }

    CloseWindow();
    FreeGui(gui);

    return 0;
}

static MenuAction maction = MACTION_COUNT;

void handleMenubar(Gui *gui) {
    if (maction != MACTION_COUNT) {
        if (maction == MACTION_SAVE_FILE) {
            ExportImage(gui->curTab->curLayer->img, "_temp.png");
            TraceLog(LOG_WARNING, "[+] Saved file as `_temp.png`");
        }
    }
}

void LayoutLogic(Gui *gui) {

    if (GuiIsLocked())
        GuiUnlock();

    SyncTabData(gui->curTab, &gui->state->menubar, &gui->state->statusbar);

    handleMenubar(gui);

    bool menuBarOpen = gui->state->menubar.menuOpen;
    bool sliderHover = gui->curTab->state->dtb.sliderHover;
    if (menuBarOpen || sliderHover) {
        GuiLock();
    }
    ColorBarLogic(&gui->curTab->state->cb);
    if (sliderHover) {
        GuiUnlock();
    }
    HandleDToolsShortcuts(&gui->curTab->state->dtb);
    if (sliderHover) {
        GuiLock();
    }

    CanvasLogic(&gui->curTab->state->cvs);
    LayerBarLogic(&gui->curTab->state->lb);
}

int tabActive = 0;

void LayoutDraw(Gui *gui) {
    /*
bool menuBarOpen = gui->state->menubar.menuOpen;
bool sliderHover = gui->curTab->state->dtb.sliderHover;

SyncTabData(gui->curTab);

if (menuBarOpen || sliderHover) {
    GuiLock();
}

float py = gui->state->menubar.prop.bounds.y +
           gui->state->menubar.prop.bounds.height + 5;
float px = gui->state->menubar.prop.bounds.x + 5;
float pw = gui->state->menubar.prop.bounds.width - 10;
float ph =
    GetScreenHeight() - gui->state->statusbar.prop.bounds.height - 5 - py;

DrawRectangleRec((Rectangle){px, py, pw, ph}, ColorGrayLighter);
DrawRectangleLinesEx((Rectangle){px, py, pw, ph}, 2, ColorWhite);
CanvasDraw(&gui->curTab->state->cvs);
ColorBarDraw(&gui->curTab->state->cb);
StatusBar(&gui->state->statusbar);

if (sliderHover)
    GuiUnlock();

DrawToolbar(&gui->curTab->state->dtb);

LayerBarDraw(&gui->curTab->state->lb);
if (sliderHover)
    GuiLock();

if (menuBarOpen)
    GuiUnlock();

maction = MenuBar(&gui->state->menubar);

if (menuBarOpen)
    GuiLock();
    */

    BpRoundedPanel(gui->curTab->tabPanel, 2, 0.07, false);
    StatusBar(&gui->state->statusbar);
    maction = MenuBar(&gui->state->menubar);

    DrawFPS(200, 50);
}
