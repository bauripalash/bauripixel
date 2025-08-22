#include "include/components.h"
#include "include/defaults.h"
#include "include/gui.h"
#include "include/menuinfo.h"
#include "include/options.h"
#include "include/tabs.h"
#include "include/theme.h"
#include "include/widgets/canvas.h"
#include "include/widgets/drawtoolbar.h"
#include "include/widgets/layerbar.h"
#include <stdbool.h>

#define STB_DS_IMPLEMENTATION
#include "external/stb/stb_ds.h"

#include "external/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

void LayoutDraw(Gui *gui);
void LayoutLogic(Gui *gui);

static int frameCounter = 0;

int RunApp() {
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(
        FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE
    );
    InitWindow(DEF_WIN_WIDTH, DEF_WIN_HEIGHT, "BauriPixel");
    // SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetExitKey(KEY_NULL);

    SetWindowMinSize(DEF_MIN_WIN_WIDTH, DEF_MIN_WIN_HEIGHT);
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
    initTab->index = 0;

    AddToTabList(gui->tabList, initTab);
    gui->curTab = gui->tabList->tabs[0];

    gui->state->statusbar.colorbar = &gui->curTab->state->cb;
    gui->state->statusbar.canvas = &gui->curTab->state->cvs;
    gui->state->statusbar.layerbar = &gui->curTab->state->lb;

    SetStatusBarPosition(&gui->state->statusbar, 0, DEF_STATUSBAR_HEIGHT);

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 5);

    SetupTabData(gui->curTab, &gui->state->menubar, &gui->state->statusbar);
    while (!WindowShouldClose()) {

        if (IsWindowResized()) {
        }

        LayoutLogic(gui);
        BeginDrawing();
        {

            ClearBackground(GetColor(OptThemeGet(T_APP_BG)));
            LayoutDraw(gui);
        }
        EndDrawing();
        frameCounter++;
    }

    FreeGui(gui);
    CloseWindow();
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
    bool layerpopup = gui->curTab->state->lb.anypopup;
    if (menuBarOpen || sliderHover || layerpopup) {
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
    if (layerpopup) {
        GuiUnlock();
    }
    LayerBarLogic(&gui->curTab->state->lb);
    if (layerpopup) {
        GuiLock();
    }
}

int tabActive = 0;

int tab0 = true;
int tab1 = false;
int tab2 = false;

void TabItemsDraw(Gui *gui) {
    int tabCount = gui->tabList->count;
    for (int t = 0; t < tabCount; t++) {
        TabObj *tab = gui->tabList->tabs[t];
        int result = BpTabItem(
            tab->tabPanel, TextFormat("Untitled %d", t), tab->index, 150,
            gui->curTab->index == tab->index
        );
        if (result == 0) {
            TraceLog(LOG_ERROR, "Open Tab %d", tab->index);
            gui->curTab = tab;
        } else if (result == 1) {
            TraceLog(LOG_ERROR, "Close Tab %d", tab->index);
        }
    }
}

void LayoutDraw(Gui *gui) {
    bool menuOpen = gui->state->menubar.menuOpen;
    bool sizeSliderHover = gui->curTab->state->dtb.sliderHover;
    bool layerpopup = gui->curTab->state->lb.anypopup;

    // SyncTabData(gui->curTab, &gui->state->menubar, &gui->state->statusbar);

    if (menuOpen || sizeSliderHover || layerpopup) {
        GuiLock();
    }

    int ogPanel = OptThemeGetSet(T_PANEL_BG, OptThemeGet(T_TAB_PANEL_BG));
    BpPanelBorder(gui->curTab->tabPanel, 3);
    OptThemeSet(T_PANEL_BG, ogPanel);

    TabItemsDraw(gui);

    CanvasDraw(&gui->curTab->state->cvs);
    ColorBarDraw(&gui->curTab->state->cb);
    StatusBar(&gui->state->statusbar);

    if (sizeSliderHover) {
        GuiUnlock();
    }

    DrawToolbar(&gui->curTab->state->dtb);

    if (sizeSliderHover) {
        GuiLock();
    }

    if (layerpopup) {
        GuiUnlock();
    }
    LayerBarDraw(&gui->curTab->state->lb);
    if (layerpopup) {
        GuiLock();
    }
    if (menuOpen) {
        GuiUnlock();
    }
    maction = MenuBar(&gui->state->menubar);
    if (menuOpen) {
        GuiLock();
    }

    /*
    Rectangle rect = {100,100,500,500};
    int ogBorder = OptThemeGetSet(T_PANEL_BORDER, HexColorVWhite);
    BpPanelOnlyBorder(rect, 8);
    DrawRectangleLinesEx(rect, 1, MAROON);

    OptThemeSet(T_PANEL_BORDER, ogBorder);
    */
    // DrawFPS(200, 50);
}
