#include "include/colors.h"
#include "include/components.h"
#include "include/defaults.h"
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

void LayoutDraw(Gui *gui);
void LayoutLogic(Gui *gui);

static int frameCounter = 1;

int RunApp() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(
        FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE
    );
    InitWindow(DEF_WIN_WIDTH, DEF_WIN_HEIGHT, "BauriPixel");
    SetWindowState(FLAG_WINDOW_MAXIMIZED);

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

    AddColorToTab(initTab, ColorVGray);
    AddColorToTab(initTab, ColorVGrayLight);
    AddColorToTab(initTab, ColorVGreen);
    AddColorToTab(initTab, ColorVWhite);

    AddToTabList(gui->tabList, initTab);
    gui->curTab = gui->tabList->tabs[0];

    gui->state->statusbar.colorbar = &gui->curTab->state->cb;
    gui->state->statusbar.canvas = &gui->curTab->state->cvs;

    SetStatusBarPosition(&gui->state->statusbar, 0, DEF_STATUSBAR_HEIGHT);

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 5);

    SetupTabData(gui->curTab, &gui->state->menubar, &gui->state->statusbar);
    while (!WindowShouldClose()) {

        // TraceLog(LOG_ERROR, "==> F %d <==", frameCounter);
        Vector2 winSize = {
            GetScreenWidth(),
            GetScreenHeight(),
        };

        // TraceVector(winSize, "WinSIZE ->");

        LayoutLogic(gui);
        BeginDrawing();
        {
            ClearBackground(GetColor(OptThemeGet(T_APP_BG)));
            LayoutDraw(gui);
        }
        EndDrawing();
        frameCounter++;
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

int tab0 = true;
int tab1 = false;
int tab2 = false;

void TabItemsDraw(Gui *gui) {
    int tabCount = gui->tabList->count;
    for (int t = 0; t < tabCount; t++) {
        TabObj *tab = gui->tabList->tabs[t];
        // SetupTabData(tab, &gui->state->menubar, &gui->state->statusbar);
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

    // SyncTabData(gui->curTab, &gui->state->menubar, &gui->state->statusbar);

    if (menuOpen || sizeSliderHover) {
        GuiLock();
    }
    int ogPanel = OptThemeGet(T_PANEL_BG);
    OptThemeSet(T_PANEL_BG, OptThemeGet(T_TAB_PANEL_BG));

    // BpRoundedPanel(gui->curTab->tabPanel, 2, 0.0f, false);
    // BpSimplePanel(gui->curTab->tabPanel, 3, (Side){}, (Side){});
    BpPanelBorder(gui->curTab->tabPanel, 3);
    // BpPanelNoBorder(gui->curTab->tabPanel);

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

    LayerBarDraw(&gui->curTab->state->lb);
    if (menuOpen) {
        GuiUnlock();
    }
    maction = MenuBar(&gui->state->menubar);
    if (menuOpen) {
        GuiLock();
    }

    // BpDummyToggleButton((Rectangle){200, 100, 40, 40}, false);
    // BpDummyFlatPanel((Rectangle){200,100,40,40}, 2, (Vector4){0,0});
    // BpSimpleButton((Rectangle){200, 100, 100, 50}, 3);

    // BpSimplePanel((Rectangle){200, 300, 200, 300}, 8, (Side){}, SideNone());
    //

    // DrawText(TextFormat("F %d", frameCounter), 500, 500, 16, RED);

    // DrawFPS(200, 50);
}
