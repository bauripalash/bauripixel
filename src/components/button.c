#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/drawtools.h"
#include "../include/options.h"
#include <stdbool.h>

#define ICON_WIDTH 32

bool BpToggleButton(Rectangle bounds, int thick, bool active) {
    int clrBorder = OptThemeGet(T_TOGGLE_BRDR);
    int clrBg = OptThemeGet(T_TOGGLE_BG);

    bool locked = GuiIsLocked();
    Vector2 mpos = GetMousePosition();
    bool hover = !locked && CheckCollisionPointRec(mpos, bounds);
    bool clicked = hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (clicked || active) {
        clrBorder = OptThemeGet(T_TOGGLE_CLKD_BRDR);
    }

    Rectangle innerRect = {
        bounds.x + thick * 2,
        bounds.y + thick * 2,
        bounds.width - thick * 4,
        bounds.height - thick * 4,
    };

    int ogBrdr = OptThemeGetSet(T_PANEL_BORDER, clrBorder);
    int ogBg = OptThemeGetSet(T_PANEL_BG, clrBg);

    if (clicked || active) {
        innerRect.x -= thick;
        innerRect.y -= thick;
        innerRect.width += thick * 2;
        innerRect.height += thick * 2;
    }

    Rectangle safeRect = {
        innerRect.x + thick, innerRect.y + thick, innerRect.width - thick * 2,
        innerRect.height - thick * 2
    };

    Vector2 iconCenter = {
        ((safeRect.width - ICON_WIDTH) / 2.0f),
        ((safeRect.height - ICON_WIDTH) / 2.0f)
    };

    OptThemeSet(T_PANEL_BORDER, clrBorder);

    BpSimplePanel(innerRect, thick, SideAll(), SideNone());

    OptThemeSet(T_PANEL_BORDER, ogBrdr);
    OptThemeSet(T_PANEL_BG, ogBg);

    return clicked;
}

bool BpSimpleButton(Rectangle bounds, int thick) {
    Color brdr = ColorTPinkLight;
    Color bg = GetColor(0x292831ff);
    Color shadow = GetColor(0x270022ff);
    bool locked = GuiIsLocked();
    Vector2 mpos = GetMousePosition();
    bool hover = !locked && CheckCollisionPointRec(mpos, bounds);
    bool clicked = hover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    Rectangle rect = {bounds.x, bounds.y, bounds.width, bounds.height};

    if (clicked) {
        rect.y += thick;
    }

    DrawRectangleRec(
        (Rectangle){rect.x + thick, rect.y, rect.width - thick * 2, thick},
        brdr
    ); // Top Border

    DrawRectangleRec(
        (Rectangle){rect.x, rect.y + thick, thick, rect.height - thick * 2},
        brdr
    ); // Left Border

    DrawRectangleRec(
        (Rectangle){rect.x + rect.width - thick, rect.y + thick, thick,
                    rect.height - thick * 2},
        brdr
    ); // Right Border

    DrawRectangleRec(
        (Rectangle){bounds.x + thick, bounds.y + bounds.height - thick,
                    bounds.width - thick * 2, thick},
        brdr
    ); // Bottom Border

    DrawRectangleRec(
        (Rectangle){
            rect.x + thick, rect.y + thick, rect.width - thick * 2,
            rect.height - thick * 2

        },
        bg
    );

    DrawRectangleLinesEx(
        (Rectangle){
            rect.x + thick * 2, rect.y + thick * 2, rect.width - thick * 4,
            rect.height - thick * 4

        },
        thick, Fade(bg, 0.5)
    );

    DrawRectangleRec(
        (Rectangle){
            rect.x + thick,
            rect.y + thick,
            thick,
            thick,
        },
        brdr
    ); // TL Dot

    DrawRectangleRec(
        (Rectangle){
            rect.x + rect.width - thick * 2,
            rect.y + thick,
            thick,
            thick,
        },
        brdr
    ); // TR Dot

    DrawRectangleRec(
        (Rectangle){rect.x + thick, rect.y + rect.height - thick * 2, thick,
                    thick},
        brdr
    ); // BL Dot

    DrawRectangleRec(
        (Rectangle){rect.x + rect.width - thick * 2,
                    rect.y + rect.height - thick * 2, thick, thick},
        brdr
    ); // BR Dot
    if (!clicked) {
        DrawRectangleRec(
            (Rectangle){rect.x + thick, rect.y + rect.height,
                        rect.width - thick * 2, thick},
            shadow
        ); // Bottom Border Shadow
        DrawRectangleRec(
            (Rectangle){rect.x, rect.y + rect.height - thick, thick, thick},
            shadow
        ); // BL Dot Shadow

        DrawRectangleRec(
            (Rectangle){rect.x + rect.width - thick,
                        rect.y + rect.height - thick, thick, thick},
            shadow
        ); // BR Dot Shadow
    }

    return false;
}
void BpBrushShapeButton(
    Rectangle bounds, BrushShape *shape, const void *tools
) {
    bool clicked = BpToggleButton(bounds, 2, false);
    int shapeWidth = bounds.height - 4;

    if (*shape == BSP_SQAURE) {
        if (clicked)
            *shape = BSP_CIRCLE;
    } else {
        if (clicked)
            *shape = BSP_SQAURE;
    }
}

int GetActiveToolIndex(const ToolInfo *tools, DrawTool active, int num) {
    for (int i = 0; i < num; i++) {
        if (tools[i].tool == active) {
            return i;
        }
    }

    return -1;
}

DrawTool BpDToolButton(
    Rectangle bounds, DrawTool active, bool *showOther, int num,
    const ToolInfo *tools
) {
    DrawTool clicked = active;
    int thick = 2;

    int activeIndex = GetActiveToolIndex(tools, active, num);
    Color iconBg = GetColor(OptThemeGet(T_DTOOL_ICON_BG));
    Color iconFg = GetColor(OptThemeGet(T_DTOOL_ICON_FG));

    Rectangle btnRect = {bounds.x, bounds.y, bounds.width, bounds.height};
    Vector2 iconCenter = {
        ((btnRect.width - ICON_WIDTH) / 2.0f),
        ((btnRect.height - ICON_WIDTH) / 2.0f)
    };
    ToolInfo first = tools[0];

    if (activeIndex >= 0) {
        first = tools[activeIndex];
    }

    if (BpToggleButton(btnRect, thick, first.tool == active)) {

        if (activeIndex == -1)
            clicked = first.tool;
        *showOther = false;
    }

    GuiDrawIcon(
        first.iconBottom, btnRect.x + iconCenter.x, btnRect.y + iconCenter.y, 2,
        iconBg
    );

    GuiDrawIcon(
        first.iconTop, btnRect.x + iconCenter.x, btnRect.y + iconCenter.y, 2,
        iconFg
    );

    if (!GuiIsLocked() && CheckCollisionPointRec(GetMousePosition(), btnRect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        *showOther = !*showOther;
    }

    if (num > 1) {
        btnRect.x += (btnRect.width + 10);
        int indexToSkip = (activeIndex >= 0) ? activeIndex : 0;
        int otherNum = num - 1;
        int list[otherNum];
        int listIndex = 0;
        Rectangle otherBtnRect = {
            btnRect.x,
            btnRect.y,
            btnRect.width + thick * 2,
            btnRect.height + thick * 2,
        };

        otherBtnRect.y += ((btnRect.height - otherBtnRect.height) / 2.0f);

        iconCenter = (Vector2){((otherBtnRect.width - ICON_WIDTH) / 2.0f),
                               ((otherBtnRect.height - ICON_WIDTH) / 2.0f)};

        for (int n = 0; n < num; n++) {
            if (n == indexToSkip) {
                continue;
            }

            list[listIndex++] = n;
        }

        if (*showOther) {
            for (int i = 0; i < otherNum; i++) {
                otherBtnRect.x += (i * otherBtnRect.width) + 10 * i;
                ToolInfo tool = tools[list[i]];

                if (BpToggleButton(otherBtnRect, thick, false)) {
                    clicked = tool.tool;
                    *showOther = false;
                }
                GuiDrawIcon(
                    tool.iconBottom, otherBtnRect.x + iconCenter.x,
                    otherBtnRect.y + iconCenter.y, 2, iconBg
                );
                GuiDrawIcon(
                    tool.iconTop, otherBtnRect.x + iconCenter.x,
                    otherBtnRect.y + iconCenter.y, 2, iconFg
                );
            }
        }
    }

    if (!GuiIsLocked() &&
        (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
         IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) &&
        !CheckCollisionPointRec(GetMousePosition(), bounds)) {
        *showOther = false;
    }

    return clicked;
}
