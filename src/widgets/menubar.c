#include "../include/widgets/menubar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/components.h"
#include "../include/defaults.h"
#include <stdbool.h>

#define MENUBAR_PADDING 5
#define ENTRY_MARGIN    5

#define ResetMenu()     (state->curMenu = TMENU_COUNT)
#define IncreaseItemY() (rect.y += state->font.baseSize + ENTRY_MARGIN)
#define MakeMenuRect()                                                         \
    ((Rectangle){bounds.x + ENTRY_MARGIN, bounds.y + ENTRY_MARGIN,             \
                 bounds.width - ENTRY_MARGIN * 2, state->font.baseSize})

static TopMenuEntry TopMenus[TMENU_COUNT] = {
    {"File", TMENU_FILE, 3, 200, false},
    {"Help", TMENU_HELP, 1, 100, false},
};

MenuBarState NewMenuBar() {
    MenuBarState state = {0};

    state.prop = NewWidgetProp();
    state.height = DEF_MENUBAR_HEIGHT;
    state.prop.bounds.x = 0;
    state.prop.bounds.y = 0;
    state.prop.bounds.height = state.height;
    state.font = GuiGetFont();
    state.curMenu = TMENU_COUNT;

    state.menuOpen = false;

    return state;
}

static void updateBounds(MenuBarState *state) {
    state->prop.bounds.x = 0;
    state->prop.bounds.y = 0;
    state->prop.bounds.width = GetScreenWidth();
    state->prop.bounds.height = state->height;
}

MenuAction drawFileMenu(MenuBarState *state, Rectangle bounds) {

    BpPanelBorder(bounds, 2);
    Rectangle rect = MakeMenuRect();
    if (BpLabelButton(rect, GuiIconText(ICON_FILE_NEW, "New"))) {
        ResetMenu();
        return MACTION_NEW_FILE;
    }

    IncreaseItemY();
    if (BpLabelButton(rect, GuiIconText(ICON_FILE_OPEN, "Open"))) {
        ResetMenu();
        return MACTION_OPEN_FILE;
    }

    IncreaseItemY();
    if (BpLabelButton(rect, GuiIconText(ICON_FILE_SAVE, "Save"))) {
        ResetMenu();
        return MACTION_SAVE_FILE;
    }

    return MACTION_COUNT;
}

MenuAction drawHelpMenu(MenuBarState *state, Rectangle bounds) {

    BpPanelBorder(bounds, 2);
    Rectangle rect = MakeMenuRect();

    if (BpLabelButton(rect, "About")) {
        ResetMenu();
        return MACTION_SHOW_ABOUT;
    }

    IncreaseItemY();

    return MACTION_COUNT;
}

#define MENU_ITEM_MARGIN 10

MenuAction openMenuPanel(MenuBarState *state, TopMenuInfo m, Rectangle bounds) {
    if (m == TMENU_FILE) {
        return drawFileMenu(state, bounds);
    } else if (m == TMENU_HELP) {
        return drawHelpMenu(state, bounds);
    }

    return MACTION_COUNT;
}

bool isMenuOpen(MenuBarState *state) { return state->curMenu != TMENU_COUNT; }

bool menuButton(
    MenuBarState *state, TopMenuEntry menu, float *posX, MenuAction *action
) {
    float btnWidth = GuiGetTextWidth(menu.name) + state->font.baseSize / 2.0f;
    Rectangle btnRect = {
        *posX + MENUBAR_PADDING, state->prop.bounds.y, btnWidth,
        state->prop.bounds.height
    };
    bool clicked =
        BpTopMenuBtn(btnRect, menu.name, menu.info == state->curMenu);
    if (clicked) {
        if (state->curMenu == menu.info) {
            ResetMenu();
        } else {
            state->curMenu = menu.info;
        }
    }

    if (state->curMenu == menu.info) {
        Vector2 mpos = GetMousePosition();
        Rectangle panelRect = {
            btnRect.x, btnRect.y + btnRect.height, menu.width + ENTRY_MARGIN,
            (menu.itemCount * (state->font.baseSize + ENTRY_MARGIN)) +
                ENTRY_MARGIN
        };

        BpPanelBorder(panelRect, 2);
        *action = openMenuPanel(state, menu.info, panelRect);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            !CheckCollisionPointRec(mpos, panelRect) &&
            !CheckCollisionPointRec(mpos, btnRect)) {
            ResetMenu();
        }
    }
    *posX += btnWidth + MENUBAR_PADDING;
    return clicked;
}

MenuAction MenuBar(MenuBarState *state) {
    updateBounds(state);
    Vector2 mpos = GetMousePosition();

    Rectangle bounds = state->prop.bounds;
    if (CheckCollisionPointRec(mpos, bounds)) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    Rectangle rect = {
        bounds.x + MENUBAR_PADDING, bounds.y, bounds.width, bounds.height
    };

    float posX = rect.x;
    MenuAction action = MACTION_COUNT;
    menuButton(state, TopMenus[0], &posX, &action);
    menuButton(state, TopMenus[1], &posX, &action);
    state->menuOpen = isMenuOpen(state);

    return action;
}
