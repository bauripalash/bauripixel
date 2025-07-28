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

    state.mFileClicked = false;

    return state;
}

static void updateBounds(MenuBarState *state) {
    state->prop.bounds.x = 0;
    state->prop.bounds.y = 0;
    state->prop.bounds.width = GetScreenWidth();
    state->prop.bounds.height = state->height;
}

Rectangle drawFileMenu(MenuBarState *state, Vector2 position) {

    Vector2 panelPos = {
        position.x,
        position.y + state->height + 5,
    };

    Rectangle panelBounds = {panelPos.x, panelPos.y, 100, 5 * 16};

    BpMenuBarPanel(panelPos, panelBounds.width, 5, 0.125);

    Rectangle rect = {panelPos.x + 5, panelPos.y + 5, panelBounds.width, 16};
    GuiLabelButton(rect, GuiIconText(ICON_FILE_NEW, "New"));

	rect.y += rect.height + 5;
	GuiLabelButton(rect, GuiIconText(ICON_FILE_OPEN, "Open"));

    return panelBounds;
}

Rectangle drawHelpMenu(MenuBarState *state, Vector2 position) {
    Vector2 panelPos = {
        position.x,
        position.y + state->height + 5,
    };

    Rectangle panelBounds = {panelPos.x, panelPos.y, 200, 5 * 16};

    BpMenuBarPanel(panelPos, 200, 5, 0.125);

    return panelBounds;
}

#define MENU_ITEM_MARGIN 10
static bool showFileMenu = false;

typedef enum MenuEnum {
    MENU_FILE = 0,
    MENU_HELP = 1,
} MenuEnum;

static struct {
    const char *n;
    int items;
    bool clicked;
    MenuEnum e;
} MenuInfo[] = {{"File", 4, false, MENU_FILE}, {"Help", 3, false, MENU_HELP}};

Rectangle openMenuPanel(MenuBarState *state, MenuEnum m, Vector2 pos) {
    if (m == MENU_FILE) {
        return drawFileMenu(state, pos);
    } else if (m == MENU_HELP) {
        return drawHelpMenu(state, pos);
    }

    return (Rectangle){0};
}

void MenuBar(MenuBarState *state) {
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

    BpRoundedPanel(bounds, 0.125);
    Font font = state->font;
    int fontSize = font.baseSize;
    Rectangle rect = {
        bounds.x + MENUBAR_PADDING, bounds.y, bounds.width, bounds.height
    };

    float posX = rect.x;

    int menuCount = 2;
    for (int mi = 0; mi < menuCount; mi++) {
        const char *menuT = MenuInfo[mi].n;
        Rectangle btnRect = {
            posX, rect.y, MeasureTextEx(font, menuT, fontSize, 0).x, rect.height
        };

        bool clkd = GuiLabelButton(btnRect, menuT) != 0;
        if (clkd) {
            for (int i = 0; i < menuCount; i++) {
                MenuInfo[i].clicked = false;
            }
            MenuInfo[mi].clicked = !MenuInfo[mi].clicked;
        }
        Rectangle panelRect = (Rectangle){-1};

        if (MenuInfo[mi].clicked) {
            panelRect =
                openMenuPanel(state, MenuInfo[mi].e, (Vector2){posX, rect.y});
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            !CheckCollisionPointRec(mpos, btnRect) &&
            !CheckCollisionPointRec(mpos, panelRect)) {
            MenuInfo[mi].clicked = false;
        }

        posX += btnRect.width + MENUBAR_PADDING * 2;
    }

    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ogLabelN);
    GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ogLabelF);
    GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ogLabelP);
    GuiSetStyle(LABEL, TEXT_COLOR_DISABLED, ogLabelD);
}
