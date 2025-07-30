#include "../include/widgets/menubar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/utils.h"
#include <stdbool.h>

#define DEF_MENUBAR_HEIGHT 25
#define MENUBAR_PADDING    5

MenuBarState NewMenuBar() {
    MenuBarState state = {0};

    state.prop = NewWidgetProp();
    state.height = DEF_MENUBAR_HEIGHT;
    state.prop.bounds.x = 0;
    state.prop.bounds.y = 0;
    state.prop.bounds.height = state.height;
    state.font = GuiGetFont();

    state.menuOpen = false;

    TopMenuEntry menus[] = {
        {"File", TMENU_FILE, 3, 200, false},
        {"Help", TMENU_HELP, 1, 100, false},
    };

    for (int i = 0; i < ArrCount(menus); i++) {
        state.menus[i] = menus[i];
    }

    return state;
}

static void updateBounds(MenuBarState *state) {
    state->prop.bounds.x = 0;
    state->prop.bounds.y = 0;
    state->prop.bounds.width = GetScreenWidth();
    state->prop.bounds.height = state->height;
}

#define ENTRY_MARGIN 5

MenuAction drawFileMenu(MenuBarState *state, Rectangle bounds) {

    BpRoundedFlatPanel(bounds, 0.125);

    float yInc = state->font.baseSize + ENTRY_MARGIN;

    Rectangle rect = {
        bounds.x + ENTRY_MARGIN, bounds.y + ENTRY_MARGIN, bounds.width,
        state->font.baseSize
    };
    if (GuiLabelButton(rect, GuiIconText(ICON_FILE_NEW, "New")) != 0) {
        return MACTION_NEW_FILE;
    }

    rect.y += yInc;
    if (GuiLabelButton(rect, GuiIconText(ICON_FILE_OPEN, "Open")) != 0) {
        return MACTION_OPEN_FILE;
    }

    rect.y += yInc;
    if (GuiLabelButton(rect, GuiIconText(ICON_FILE_SAVE, "Save")) != 0) {
        return MACTION_SAVE_FILE;
    }

    return MACTION_COUNT;
}

MenuAction drawHelpMenu(MenuBarState *state, Rectangle bounds) {

    BpRoundedFlatPanel(bounds, 0.125);

    return MACTION_COUNT;
}

#define MENU_ITEM_MARGIN 10
static bool showFileMenu = false;

MenuAction openMenuPanel(MenuBarState *state, TopMenuInfo m, Rectangle bounds) {
    if (m == TMENU_FILE) {
        return drawFileMenu(state, bounds);
    } else if (m == TMENU_HELP) {
        return drawHelpMenu(state, bounds);
    }

    return MACTION_COUNT;
}

bool isMenuOpen(MenuBarState *state) {
    for (int i = 0; i < ArrCount(state->menus); i++) {
        if (state->menus[i].active) {
            return true;
        }
    }

    return false;
}

MenuAction MenuBar(MenuBarState *state) {
    updateBounds(state);

    int ogLabelN = GuiGetStyle(LABEL, TEXT_COLOR_NORMAL);
    int ogLabelF = GuiGetStyle(LABEL, TEXT_COLOR_FOCUSED);
    int ogLabelP = GuiGetStyle(LABEL, TEXT_COLOR_PRESSED);
    int ogLabelD = GuiGetStyle(LABEL, TEXT_COLOR_DISABLED);
    Vector2 mpos = GetMousePosition();

    Color tClr = ColorWhite;

    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(ColorWhite));
    GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ColorToInt(ColorGrayLightest));
    GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ColorToInt(ColorYellow));
    GuiSetStyle(LABEL, TEXT_COLOR_DISABLED, ColorToInt(ColorGrayLightest));

    Rectangle bounds = state->prop.bounds;
    if (CheckCollisionPointRec(mpos, bounds)) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    MenuAction resultAction = MACTION_COUNT;

    BpRoundedPanel(bounds, 0.125);
    Font font = state->font;
    int fontSize = font.baseSize;
    Rectangle rect = {
        bounds.x + MENUBAR_PADDING, bounds.y, bounds.width, bounds.height
    };

    float posX = rect.x;

    int menuCount = ArrCount(state->menus);
    for (int mi = 0; mi < menuCount; mi++) {

        const char *menuT = state->menus[mi].name;
        Rectangle btnRect = {
            posX, rect.y, MeasureTextEx(font, menuT, fontSize, 0).x, rect.height
        };

        bool clkd = GuiLabelButton(btnRect, menuT) != 0;
        if (clkd) {
            for (int i = 0; i < menuCount; i++) {
                state->menus[i].active = false;
            }
            state->menus[mi].active = !state->menus[mi].active;
        }
        Rectangle panelRect =
            (Rectangle){btnRect.x, btnRect.y + rect.height,
                        state->menus[mi].width,
                        state->menus[mi].itemCount * fontSize +
                            ((state->menus[mi].itemCount + 1) * ENTRY_MARGIN)};

        if (state->menus[mi].active) {
            resultAction =
                openMenuPanel(state, state->menus[mi].info, panelRect);
        }

        if (resultAction != MACTION_COUNT) {
            state->menus[mi].active = false;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            !CheckCollisionPointRec(mpos, btnRect) &&
            !CheckCollisionPointRec(mpos, panelRect)) {
            state->menus[mi].active = false;
        }

        posX += btnRect.width + MENUBAR_PADDING * 2;
    }

    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ogLabelN);
    GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ogLabelF);
    GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ogLabelP);
    GuiSetStyle(LABEL, TEXT_COLOR_DISABLED, ogLabelD);

    state->menuOpen = isMenuOpen(state);

    return resultAction;
}
