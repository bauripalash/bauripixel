#include "include/colors.h"
#include "include/gui.h"
#include "include/menuinfo.h"
#include "include/tabs.h"
#include "include/theme.h"
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

    AddColorToTab(initTab, ColorBlack);
    AddColorToTab(initTab, ColorPurple);
    AddColorToTab(initTab, ColorRedDark);
    AddColorToTab(initTab, ColorOrange);
    AddColorToTab(initTab, ColorYellow);
    AddColorToTab(initTab, ColorGreenLightest);
    AddColorToTab(initTab, ColorGreenLighter);
    AddColorToTab(initTab, ColorGreenDarkest);

    AddColorToTab(initTab, ColorBlueDarkest);
    AddColorToTab(initTab, ColorBlueLighter);
    AddColorToTab(initTab, ColorBlueLightest);
    AddColorToTab(initTab, ColorCyan);

    AddColorToTab(initTab, ColorWhite);
    AddColorToTab(initTab, ColorGrayLightest);
    AddColorToTab(initTab, ColorGrayLighter);
    AddColorToTab(initTab, ColorGrayDarkest);
    AddColorToTab(initTab, RAYWHITE);

    AddToTabList(gui->tabList, initTab);
    gui->curTab = gui->tabList->tabs[0];

    gui->state->statusbar.colorbar = &gui->curTab->state->cb;
    gui->state->statusbar.canvas = &gui->curTab->state->cvs;

    float sbarHeight = 30;

    SetStatusBarPosition(&gui->state->statusbar, 0, sbarHeight);

    SyncTabData(gui->curTab);

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 5);

    while (!WindowShouldClose()) {
        LayoutLogic(gui);
        BeginDrawing();
        {
            ClearBackground(ColorXDarkGray1);
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

    SyncTabData(gui->curTab);

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
}

void LayoutDraw(Gui *gui) {
    bool menuBarOpen = gui->state->menubar.menuOpen;
    bool sliderHover = gui->curTab->state->dtb.sliderHover;

    SyncTabData(gui->curTab);

    if (menuBarOpen || sliderHover) {
        GuiLock();
    }
    CanvasDraw(&gui->curTab->state->cvs);
    ColorBarDraw(&gui->curTab->state->cb);
    StatusBar(&gui->state->statusbar);

    if (sliderHover)
        GuiUnlock();

    DrawToolbar(&gui->curTab->state->dtb);

    if (sliderHover)
        GuiLock();

    if (menuBarOpen)
        GuiUnlock();

    maction = MenuBar(&gui->state->menubar);

    if (menuBarOpen)
        GuiLock();

    DrawFPS(200, 50);
}
