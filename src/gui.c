#include "include/gui.h"
#include "external/raylib.h"
#include "include/tabs.h"
#include "include/widgets/menubar.h"
#include "include/widgets/statusbar.h"
#include <stdlib.h>

GuiGlobalState *NewGuiState() {
    GuiGlobalState *gs = malloc(sizeof(GuiGlobalState));
    gs->menubar = NewMenuBar();
    gs->statusbar = NewStatusBar();

    return gs;
}

void FreeGuiState(GuiGlobalState *state) {
    if (state == NULL) {
        return;
    }

    // * Free States which allocates memory *

    free(state);
}

Gui *NewGui() {
    Gui *g = malloc(sizeof(Gui));

    if (g == NULL) {
        return NULL;
    }

    g->tabList = NewTabList();

    if (g->tabList == NULL) {
        free(g);
        return NULL;
    }

    g->state = NewGuiState();
    if (g->state == NULL) {
        FreeTabList(g->tabList);
        free(g);
        return NULL;
    }

    g->curFilePath = NULL; // use be added later

    return g;
}

void FreeGui(Gui *gui) {
    if (gui == NULL) {
        return;
    }

    FreeTabList(gui->tabList);
    FreeGuiState(gui->state);
    free(gui);
}
