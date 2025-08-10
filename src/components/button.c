#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/components.h"
#include "../include/drawtools.h"
#include "../include/options.h"
#include <stdbool.h>

#define ICON_WIDTH 32

void BpBrushShapeButton(
    Rectangle bounds, BrushShape *shape, const DToolInfo *tools
) {
    bool clicked = BpDummyToggleButton(bounds, false);
    int shapeWidth = bounds.height - 4;

    if (*shape == BSP_SQAURE) {
        if (clicked)
            *shape = BSP_CIRCLE;
    } else {
        if (clicked)
            *shape = BSP_SQAURE;
    }

    Vector2 iconCenter = {
        ((bounds.width - shapeWidth) / 2.0f),
        ((bounds.height - shapeWidth) / 2.0f),
    };

    Rectangle iconRect = {
        bounds.x + iconCenter.x, bounds.y + iconCenter.y,

        shapeWidth, shapeWidth
    };

    Rectangle txtSrc = {0, 0, ICON_WIDTH, ICON_WIDTH};

    Vector2 origin = Vector2Zero();

    DrawTexturePro(tools[(int)*shape].txt, txtSrc, iconRect, origin, 0, WHITE);
}

static int getActiveTool(DrawTool active, int num, const DToolInfo *tools) {
    for (int i = 0; i < num; i++) {
        if (tools[i].tool == active) {
            return i;
        }
    }
    return -1;
}

DrawTool BpToolButton(
    Rectangle bounds, DrawTool active, bool *showOther, int num,
    const DToolInfo *tools
) {

    Rectangle rect = {bounds.x, bounds.y, bounds.width, bounds.height};
    DrawTool clicked = active;

    int activeToolIndex = getActiveTool(active, num, tools);
    Rectangle btnRect = {rect.x, rect.y, rect.width, rect.height};
    Vector2 iconCenter = {
        ((btnRect.width - ICON_WIDTH) / 2.0f),
        ((btnRect.height - ICON_WIDTH) / 2.0f)
    };

    if (activeToolIndex >= 0) {
        DToolInfo tool = tools[activeToolIndex];
        if (BpDummyToggleButton(btnRect, true)) {
            *showOther = false;
        }
        btnRect.y += 3;

        DrawTexture(
            tool.txt, btnRect.x + iconCenter.x, btnRect.y + iconCenter.y, WHITE
        );

    } else {
        DToolInfo tool = tools[0];
        if (BpDummyToggleButton(btnRect, tool.tool == active)) {
            clicked = tool.tool;
            *showOther = false;
        }

        DrawTexture(
            tool.txt, btnRect.x + iconCenter.x, btnRect.y + iconCenter.y, WHITE
        );
    }

    if (CheckCollisionPointRec(GetMousePosition(), btnRect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !GuiIsLocked()) {
        *showOther = !*showOther;
    }

    if (num > 1) {
        btnRect.x += (btnRect.width + 10);

        int indexToSkip = (activeToolIndex >= 0) ? activeToolIndex : 0;

        int numOtherTools = num - 1;
        int list[numOtherTools];
        int listIndex = 0;
        for (int n = 0; n < num; n++) {
            if (n == indexToSkip)
                continue;
            list[listIndex] = n;
            listIndex++;
        }

        if (*showOther) {

            for (int i = 0; i < numOtherTools; i++) {

                btnRect.x += (i * btnRect.width) + 10 * i;
                DToolInfo tool = tools[list[i]];

                if (BpDummyToggleButton(btnRect, false)) {
                    clicked = tool.tool;
                    *showOther = false;
                }

                DrawTexture(
                    tool.txt, btnRect.x + iconCenter.x,
                    btnRect.y + iconCenter.y, WHITE
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

bool BpDummyFlatButton(Rectangle bounds) {
    bool locked = GuiIsLocked();
    Color bg = GetColor(OptThemeGet(T_BTN_BG)); // ColorGrayDarker;
    Color hoverBg = GetColor(OptThemeGet(T_BTN_HVR_BG));
    Color clickBg = GetColor(OptThemeGet(T_BTN_CLK_BG));
    Color brdr = GetColor(OptThemeGet(T_BTN_BORDER));
    Color shadowClr = GetColor(OptThemeGet(T_BTN_SHADOW));
    Color corner = GetColor(OptThemeGet(T_BTN_CORNER));

    Vector2 mpos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mpos, bounds) && !locked;
    bool clicked = hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    float thick = OptThemeGet(T_BTN_BRDR_THICK);

    Rectangle innerRect =
        (Rectangle){bounds.x + thick, bounds.y + thick,
                    bounds.width - thick * 2, bounds.height - thick * 2};

    if (clicked) {
        bg = clickBg;
    } else {
        if (hover) {
            bg = hoverBg;
        }
    }

    DrawRectangleRec(
        (Rectangle){bounds.x, bounds.y, bounds.width, bounds.height + thick},
        shadowClr
    );

    DrawRectangleRec(innerRect, bg);
    DrawRectangleLinesEx(innerRect, thick, brdr);
    DrawRectangleRec(
        (Rectangle){
            innerRect.x,
            innerRect.y,
            thick,
            thick,
        },
        corner
    ); // Top Left Dot

    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick, innerRect.y, thick,
                    thick},
        corner
    ); // Top Right Dot

    DrawRectangleRec(
        (Rectangle){
            innerRect.x,
            innerRect.y + innerRect.height - thick,
            thick,
            thick,
        },
        corner
    ); // Bottom Left Dot

    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick,
                    innerRect.y + innerRect.height - thick, thick, thick},
        corner
    ); // Bottom Right Dot

    return clicked;
}

bool BpDummyToggleButton(Rectangle bounds, bool active) {
    bool locked = GuiIsLocked();
    Color bg = GetColor(OptThemeGet(T_BTN_BG)); // ColorGrayDarker;
    Color hoverBg = GetColor(OptThemeGet(T_BTN_HVR_BG));
    Color clickBg = GetColor(OptThemeGet(T_BTN_CLK_BG));
    Color brdr = GetColor(OptThemeGet(T_BTN_BORDER));
    Color shadowClr = GetColor(OptThemeGet(T_BTN_SHADOW));
    Color corner = GetColor(OptThemeGet(T_BTN_CORNER));

    Vector2 mpos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mpos, bounds) && !locked;
    bool clicked = hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    float thick = OptThemeGet(T_BTN_BRDR_THICK);

    Rectangle innerRect =
        (Rectangle){bounds.x + thick, bounds.y + thick,
                    bounds.width - thick * 2, bounds.height - thick * 2};

    if (active) {
        innerRect.x += thick;
        innerRect.y += thick;
    }

    if (clicked) {
        if (!active) {
            innerRect.x += thick;
            innerRect.y += thick;
        }
        bg = clickBg;
    } else {
        if (hover) {
            bg = hoverBg;
        }
        if (!active) {
            DrawRectangleRounded(
                (Rectangle){bounds.x + thick, bounds.y + thick, bounds.width,
                            bounds.height},
                0.2, 0, shadowClr
            );
        }
    }

    DrawRectangleRec(innerRect, bg);
    DrawRectangleLinesEx(innerRect, thick, brdr);

    DrawRectangleRec(
        (Rectangle){
            innerRect.x,
            innerRect.y,
            thick,
            thick,
        },
        corner
    ); // Top Left Dot

    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick, innerRect.y, thick,
                    thick},
        corner
    ); // Top Right Dot

    DrawRectangleRec(
        (Rectangle){
            innerRect.x,
            innerRect.y + innerRect.height - thick,
            thick,
            thick,
        },
        corner
    ); // Bottom Left Dot

    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick,
                    innerRect.y + innerRect.height - thick, thick, thick},
        corner
    ); // Bottom Right Dot

    return clicked;
}
