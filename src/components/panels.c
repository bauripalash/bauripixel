#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/options.h"
#include "../include/utils.h"
#include <stdbool.h>
#include <stdio.h>

#define SHADOW_SIZE 3

static inline void DrawTLDot(Rectangle b, int tk, Color c) {
    DrawRectangleRec(
        (Rectangle){
            b.x,
            b.y,
            tk,
            tk,
        },
        c
    ); // TL Dot
}

static inline void DrawTRDot(Rectangle b, int tk, Color c) {
    DrawRectangleRec(
        (Rectangle){
            b.x + b.width - tk,
            b.y,
            tk,
            tk,
        },
        c
    ); // TR Dot
}

static inline void DrawBLDot(Rectangle b, int tk, Color c) {
    DrawRectangleRec(
        (Rectangle){b.x, b.y + b.height - tk, tk, tk},
        c
    ); // BL Dot
}

static inline void DrawBRDot(Rectangle b, int tk, Color c) {
    DrawRectangleRec(
        (Rectangle){b.x + b.width - tk, b.y + b.height - tk, tk, tk},
        c
    ); // BR Dot
}

void BpPanelNoBorderShadow(Rectangle bounds) {}

void BpPanelBorderShadow(Rectangle bounds, int thick) {}

void BpPanelNoBorder(Rectangle bounds) {
    DrawRectangleRec(bounds, GetColor(OptThemeGet(T_PANEL_BG)));
}

void BpPanelBorder(Rectangle bounds, int thick) {
    BpSimplePanel(bounds, thick, SideAll(), SideNone());
}

void BpPanelOnlyBorder(Rectangle bounds, int thick) {
    Color brdr = GetColor(OptThemeGet(T_PANEL_BORDER));
    Rectangle innerRect = {
        bounds.x + thick, bounds.y + thick, bounds.width - thick * 2,
        bounds.height - thick * 2
    };

    DrawRectangle(innerRect.x, bounds.y, innerRect.width, thick, brdr);
    DrawRectangle(bounds.x, innerRect.y, thick, innerRect.height, brdr);
    DrawRectangle(
        bounds.x + bounds.width - thick, innerRect.y, thick, innerRect.height,
        brdr
    );
    DrawRectangle(
        innerRect.x, bounds.y + bounds.height - thick, innerRect.width, thick,
        brdr
    );
    DrawTLDot(innerRect, thick, brdr);
    DrawTRDot(innerRect, thick, brdr);
    DrawBLDot(innerRect, thick, brdr);
    DrawBRDot(innerRect, thick, brdr);
}

// Note: Only One Side Border Hiding Works for now
void BpSimplePanel(Rectangle bounds, int thick, Side border, Side shadow) {
    Color brdr = GetColor(OptThemeGet(T_PANEL_BORDER));
    Color bg = GetColor(OptThemeGet(T_PANEL_BG));

    // Left + Right Borders
    DrawRectangle(
        bounds.x - thick, bounds.y, bounds.width + thick * 2, bounds.height,
        brdr
    );
    // Top + Bottom Borders;
    DrawRectangle(
        bounds.x, bounds.y - thick, bounds.width, bounds.height + thick * 2,
        brdr
    );

    // Left + Right Background
    DrawRectangle(
        bounds.x, bounds.y + thick, bounds.width, bounds.height - thick * 2, bg
    );
    // Top + Bottom Background
    DrawRectangle(
        bounds.x + thick, bounds.y, bounds.width - thick * 2, bounds.height, bg
    );

    if (!IsAllSide(border)) {
        if (border.l == -1) {
            DrawRectangle(
                bounds.x - thick, bounds.y, bounds.width + thick, bounds.height,
                bg
            );
            DrawTRDot(bounds, thick, brdr);
            DrawBRDot(bounds, thick, brdr);
        }

        if (border.t == -1) {
            DrawRectangle(
                bounds.x, bounds.y - thick, bounds.width, bounds.height + thick,
                bg
            );
            DrawBLDot(bounds, thick, brdr);
            DrawBRDot(bounds, thick, brdr);
        }

        if (border.b == -1) {
            DrawRectangle(
                bounds.x, bounds.y, bounds.width, bounds.height + thick, bg
            );
            DrawTLDot(bounds, thick, brdr);
            DrawTRDot(bounds, thick, brdr);
        }

        if (border.r == -1) {
            DrawRectangle(
                bounds.x, bounds.y, bounds.width + thick, bounds.height, bg
            );

            DrawTLDot(bounds, thick, brdr);
            DrawBLDot(bounds, thick, brdr);
        }
    }
}

float tabHeight = 30;
float tabThick = 2;
#define TABITEM_PADDING 5
int BpTabItem(
    Rectangle bounds, const char *name, int index, float tabWidth, bool active
) {
    int ogPanelBg = 0;
    if (active) {
        ogPanelBg = OptThemeGetSet(T_PANEL_BG, OptThemeGet(T_TAB_ITEM_ACTIVE));
    } else {
        ogPanelBg =
            OptThemeGetSet(T_PANEL_BG, OptThemeGet(T_TAB_ITEM_INACTIVE));
    }
    // SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    Rectangle tabRect =
        (Rectangle){bounds.x + (index * (tabWidth + tabThick)),
                    bounds.y - tabHeight, tabWidth, tabHeight + 4};
    Rectangle innerRect = {
        tabRect.x + tabThick * 2,
        tabRect.y + tabThick * 2,
        tabRect.width - (tabThick * 2) * 2,
        tabRect.height,
    };

    BpSimplePanel(tabRect, 3, (Side){.b = -1}, (Side){});

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

    OptThemeSet(T_PANEL_BG, ogPanelBg);
    Vector2 mpos = GetMousePosition();
    if (!GuiIsLocked() && !CheckCollisionPointRec(mpos, xButton) &&
        CheckCollisionPointRec(GetMousePosition(), tabRect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return 0;
    }

    if (GuiLabelButton(xButton, GuiIconText(ICON_CROSS, NULL)) != 0) {
        return 1;
    }

    return -1;
}

void BpMenuBarPanel(Vector2 position, float width, int items, float roundness) {
    Font font = GuiGetFont();
    float panelHeight = font.baseSize * items;

    Rectangle rect = {position.x, position.y, width, panelHeight};

    // BpRoundedFlatPanel(rect, roundness);
    BpPanelBorder(rect, 2);
}
