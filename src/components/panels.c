#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/options.h"
#include <stdbool.h>
#include <stdio.h>

#define SHADOW_SIZE 3

void BpDummyPanel(
    Rectangle bounds, float thick, float roundness, bool shadow, Vector4 borders
);

void BpRoundedPanel(Rectangle bounds, float thick, float shadowr, bool shadow) {
    BpDummyPanel(bounds, thick, shadowr, shadow, (Vector4){0, 0, 0, 0});
}

float tabHeight = 30;
float tabThick = 2;
#define TABITEM_PADDING 5
int BpTabItem(
    Rectangle bounds, const char *name, int index, float tabWidth, bool active
) {
    int ogPanelBg = OptThemeGet(T_PANEL_BG);
    if (!active) {
        OptThemeSet(T_PANEL_BG, HexColorGrayLighter);
    }
    Color panelBg = GetColor(OptThemeGet(T_TAB_ITEM_ACTIVE));
    if (!active) {
        OptThemeSet(T_PANEL_BG, OptThemeGet(T_TAB_ITEM_INACTIVE));
    }

    Color panelBorder = GetColor(OptThemeGet(T_PANEL_BORDER));
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    Rectangle tabRect =
        (Rectangle){bounds.x + (index * (tabWidth + tabThick)),
                    bounds.y - tabHeight, tabWidth, tabHeight + 4};
    Rectangle innerRect = {
        tabRect.x + tabThick * 2,
        tabRect.y + tabThick * 2,
        tabRect.width - (tabThick * 2) * 2,
        tabRect.height,
    };
    float tabX0 = tabRect.x;
    float tabX1 = tabRect.x + tabRect.width;

    BpDummyPanel(tabRect, 2, 0.0f, false, (Vector4){.w = -1});
    if (active) {
        DrawRectangleRec(innerRect, panelBg);
    }
    if (tabX0 == bounds.x) {
        DrawRectangleRec(
            (Rectangle){tabX0 + tabThick, tabRect.y + tabThick * 2, tabThick,
                        tabRect.height + 5},
            panelBorder
        );
    }

    if (tabX1 == bounds.x + bounds.width) {
        DrawRectangleRec(
            (Rectangle){tabX1 - tabThick * 2, tabRect.y + tabThick * 2,
                        tabThick, tabRect.height + 5},
            panelBorder
        );
    }

    Rectangle usableBounds = {
        innerRect.x + TABITEM_PADDING, tabRect.y + TABITEM_PADDING,
        innerRect.width - TABITEM_PADDING * 2,
        tabRect.height - TABITEM_PADDING * 2
    };

    Rectangle nameBounds = {
        usableBounds.x, usableBounds.y, usableBounds.width - 16,
        usableBounds.height
    };

    Rectangle xButton = {
        nameBounds.x + nameBounds.width, nameBounds.y, nameBounds.height,
        nameBounds.height
    };

    GuiLabel(nameBounds, name);

    Vector2 mpos = GetMousePosition();
    if (!GuiIsLocked() && !CheckCollisionPointRec(mpos, xButton) &&
        CheckCollisionPointRec(GetMousePosition(), tabRect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return 0;
    }

    if (GuiLabelButton(xButton, GuiIconText(ICON_CROSS, NULL)) != 0) {
        return 1;
    }
    OptThemeSet(T_PANEL_BG, ogPanelBg);

    return -1;
}

void BpRoundedPanelBorder(
    Rectangle bounds, float thick, float r, Vector4 borders
) {}

void BpRoundedFlatPanel(Rectangle bounds, float roundness) {

    DrawRectangleRounded(bounds, roundness, 0, ColorGrayDarker);
    DrawRectangleRoundedLinesEx(bounds, roundness, 0, 2, ColorGrayLightest);
}

void BpMenuBarPanel(Vector2 position, float width, int items, float roundness) {
    Font font = GuiGetFont();
    float panelHeight = font.baseSize * items;

    Rectangle rect = {position.x, position.y, width, panelHeight};

    BpRoundedFlatPanel(rect, roundness);
}

void BpDummyPanel(
    Rectangle bounds, float thick, float roundness, bool shadow, Vector4 borders
) {
    Color panelBorder = GetColor(OptThemeGet(T_PANEL_BORDER));
    Color panelBg = GetColor(OptThemeGet(T_PANEL_BG));
    Color panelShadow = GetColor(OptThemeGet(T_PANEL_SHADOW));

    Rectangle innerRect = {
        bounds.x + thick, bounds.y + thick, bounds.width - thick * 2,
        bounds.height - thick * 2
    };

    int shadowHeight = bounds.height;
    if (shadow) {
        shadowHeight += 5;
        DrawRectangleRounded(
            (Rectangle){bounds.x, bounds.y, bounds.width, shadowHeight},
            shadow ? 0.07 : roundness, 2, panelShadow
        );
    } else {
        Rectangle shadowRect =
            (Rectangle){bounds.x, bounds.y, bounds.width, bounds.height};
        if (borders.x == -1) {
            shadowRect.x += thick;
        }

        if (borders.y == -1)
            shadowRect.width -= thick;
        if (borders.z == -1)
            shadowRect.y += thick;
        if (borders.w == -1) {
            shadowRect.height -= thick;
        }
        DrawRectangleRec(shadowRect, panelShadow);
    }

    int tk = 4;
    Rectangle faceFg = {
        innerRect.x + tk, innerRect.y + tk, innerRect.width - tk * 2,
        innerRect.height - tk * 2
    };

    DrawRectangleRec(innerRect, panelBg); // Fade(ColorVGrayLight, 0.3));
    // DrawRectangleLinesEx(innerRect, thick, panelBorder);

    Vector2 tl = {innerRect.x + thick, innerRect.y + thick};
    Vector2 tr = {
        innerRect.x + innerRect.width - thick * 2, innerRect.y + thick
    };
    Vector2 bl = {innerRect.x + thick, innerRect.y + innerRect.height - thick};
    Vector2 br = {
        innerRect.x + innerRect.width - thick,
        innerRect.y + innerRect.height - thick
    };

    Color sc = ColorVGrayLight;

    if (borders.x != -1) {
        Color brdr = panelBorder;
        if (borders.x != 0) {
            brdr = GetColor((int)borders.x);
        }
        DrawRectangleRec(
            (Rectangle){innerRect.x, innerRect.y, thick, innerRect.height}, brdr
        );

        // DrawLineEx(tl, bl, thick, brdr);
    }

    if (borders.y != -1) {
        Color brdr = panelBorder;
        if (borders.y != 0) {
            brdr = GetColor((int)borders.y);
        }

        DrawRectangleRec(
            (Rectangle){innerRect.x + innerRect.width - thick, innerRect.y,
                        thick, innerRect.height},
            brdr
        );

        // DrawLineEx(tr, br, thick, panelBorder);
    }

    if (borders.z != -1) {
        Color brdr = panelBorder;
        if (borders.z != 0) {
            brdr = GetColor((int)borders.z);
        }

        // DrawLineEx(tl, tr, thick, panelBorder);
        DrawRectangleRec(
            (Rectangle){innerRect.x, innerRect.y, innerRect.width, thick}, brdr
        );
    }

    if (borders.w != -1) {
        Color brdr = panelBorder;
        if (borders.w != 0) {
            brdr = GetColor((int)borders.w);
        }
        // DrawLineEx(bl, br, thick, panelBorder);
        DrawRectangleRec(
            (Rectangle){innerRect.x, innerRect.height + innerRect.y - thick,
                        innerRect.width, thick},
            brdr
        );
    }

    DrawRectangleRec((Rectangle){innerRect.x, innerRect.y, thick, thick}, sc);
    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick, innerRect.y, thick,
                    thick},
        sc
    );
    if (borders.w != -1) {
        DrawRectangleRec(
            (Rectangle){innerRect.x, innerRect.y + innerRect.height - thick,
                        thick, thick},
            sc
        );
    }
    if (borders.w != -1) {
        DrawRectangleRec(
            (Rectangle){innerRect.x + innerRect.width - thick,
                        innerRect.y + innerRect.height - thick, thick, thick},
            sc
        );
    }
}
