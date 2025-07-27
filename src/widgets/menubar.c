#include "../include/widgets/menubar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
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
static void drawBounds(Rectangle bounds, Color bg) {
    DrawRectangleRounded(bounds, 0.125, 0, bg);
    DrawRectangleRoundedLinesEx(bounds, 0.125, 0, 2, ColorBlack);

    DrawRectangleRoundedLinesEx(
        (Rectangle){
            bounds.x + 2,
            bounds.y + 2,
            bounds.width - 4,
            bounds.height - 4,
        },
        0.125, 0, 2, ColorGrayLightest
    );
}

#define M_FILE_TXT "File"

static char *MenuTexts[] = {"File", "Edit", "Help"};

static bool MenuClicks[] = {false, false, false};

#define MENU_ITEM_MARGIN 10
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
    Font font = state->font;
    int fontSize = font.baseSize;
    Rectangle rect = {
        bounds.x + MENUBAR_PADDING, bounds.y, bounds.width, bounds.height
    };
    bool fclk = GuiLabelButton(rect, M_FILE_TXT) != 0;
    int menuItemCount = ArrCount(MenuTexts);

    for (int mi = 0; mi < menuItemCount; mi++) {
        const char *itemText = MenuTexts[mi];
        Vector2 textSize = MeasureTextEx(font, itemText, fontSize, 0);
        float msize = textSize.x;
        Rectangle btnRect = {
            rect.x + (msize + MENU_ITEM_MARGIN) * mi, rect.y,
            (msize + MENUBAR_PADDING), rect.height
        };

        bool btnClicked = GuiLabelButton(btnRect, itemText) != 0;

        if (btnClicked) {
            for (int b = 0; b < menuItemCount; b++) {
                MenuClicks[b] = false;
            }

            MenuClicks[mi] = !MenuClicks[mi];
        }

        Rectangle menuPanelRect = {
            btnRect.x, btnRect.y + btnRect.height, 200, 300
        };

        if (MenuClicks[mi]) {
            drawBounds(menuPanelRect, ColorGrayLighter);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            !CheckCollisionPointRec(mpos, btnRect) &&
            !CheckCollisionPointRec(mpos, menuPanelRect)) {
            MenuClicks[mi] = false;
        }
    }

    drawBounds(bounds, ColorFDGrayLighter);

    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ogLabelN);
    GuiSetStyle(LABEL, TEXT_COLOR_FOCUSED, ogLabelF);
    GuiSetStyle(LABEL, TEXT_COLOR_PRESSED, ogLabelP);
    GuiSetStyle(LABEL, TEXT_COLOR_DISABLED, ogLabelD);
}
