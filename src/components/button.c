#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/drawtools.h"
#include <stdbool.h>
#include <string.h>

#define ICON_WIDTH 32

bool BpDummyToogleButton(Rectangle bounds, bool active) {
    Color bg = ColorGrayDarker;
    Color activeBg = Fade(ColorGrayLighter, 0.5);
    Color brdr = ColorGrayLightest;
    Color shadow = ColorBlack;
    Vector2 mpos = GetMousePosition();

    float bx = bounds.x;
    float by = bounds.y;
    float bw = bounds.width;
    float bh = bounds.height;

    Rectangle brect = {bx, by, bw, bh};

    bool clicked = CheckCollisionPointRec(mpos, bounds) &&
                   IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !GuiIsLocked();

    if (clicked || active) {
        brect.y += 3;
        bg = activeBg;
    } else {
        DrawRectangleRounded((Rectangle){bx, by, bw, bh + 3}, 0.2, 0, shadow);
    }

    DrawRectangleRounded(brect, 0.2, 0, bg);
    DrawRectangleRoundedLinesEx(brect, 0.2, 0, 2, shadow);
    DrawRectangleRoundedLinesEx(
        (Rectangle){
            brect.x + 2,
            brect.y + 2,
            brect.width - 4,
            brect.height - 4,
        },
        0.2, 0, 2, brdr
    );

    return clicked;
}

void BpBrushShapeButton(
    Rectangle bounds, BrushShape *shape, const DToolInfo *tools
) {
    bool clicked = BpDummyToogleButton(bounds, false);
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
        if (BpDummyToogleButton(btnRect, true)) {
            *showOther = false;
        }
        btnRect.y += 3;

        DrawTexture(
            tool.txt, btnRect.x + iconCenter.x, btnRect.y + iconCenter.y, WHITE
        );

    } else {
        DToolInfo tool = tools[0];
        if (BpDummyToogleButton(btnRect, tool.tool == active)) {
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

                if (BpDummyToogleButton(btnRect, false)) {
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

bool BpDummyRaisedButton(Rectangle bounds) {
    bool locked = GuiIsLocked();
    Color bg = ColorGrayDarker;
    Color hoverBg = ColorGrayLighter;
    Color brdr = ColorGrayLightest;
    Color shadow = ColorBlack;

    Vector2 mpos = GetMousePosition();

    bool hover = CheckCollisionPointRec(mpos, bounds) && !locked;
    bool clicked = hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    Rectangle rect = {bounds.x, bounds.y, bounds.width, bounds.height - 5};

    if (clicked) {
        rect.y += 5;
    } else {
        DrawRectangleRounded(
            (Rectangle){rect.x, rect.y, rect.width, rect.height + 5}, 0.2, 0,
            shadow
        );
    }

    DrawRectangleRounded(rect, 0.2, 0, hover ? hoverBg : bg);
    DrawRectangleRoundedLinesEx(
        (Rectangle){rect.x + 2, rect.y + 2, rect.width - 4, rect.height - 4},
        0.2, 0, 2, brdr
    );

    return clicked;
}

bool BpDummyButton(Rectangle bounds) {
    bool locked = GuiIsLocked();
    Color bg = ColorGrayDarker;
    Color hoverBg = ColorGrayLighter;
    Color brdr = ColorGrayLightest;
    Color shadow = ColorBlack;

    Vector2 mpos = GetMousePosition();

    bool hover = CheckCollisionPointRec(mpos, bounds) && !locked;
    bool clicked = hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    Rectangle rect = {bounds.x, bounds.y, bounds.width, bounds.height};

    /*
DrawRectangleRoundedLinesEx(rect, 0.2, 0, 2, shadow);

DrawRectangleRounded(rect, 0.2, 0, hover ? hoverBg : bg);

DrawRectangleRoundedLinesEx(
    (Rectangle){rect.x + 2, rect.y + 2, rect.width - 4, rect.height - 4},
    0.2, 0, 2, brdr
);
    */

    float thick = 2;

    Vector2 tl = (Vector2){bounds.x + thick, bounds.y};
    Vector2 tr = (Vector2){(bounds.x + bounds.width) - thick, bounds.y};
    Vector2 bl =
        (Vector2){bounds.x + thick, (bounds.y + bounds.height) - thick};
    Vector2 br = (Vector2){(bounds.x + bounds.width) - thick,
                           (bounds.y + bounds.height) - thick};

    Rectangle innerRect = {
        bounds.x + thick, bounds.y + thick, bounds.width - thick * 2,
        bounds.height - thick * 2
    };
    DrawRectangleRounded(
        (Rectangle){bounds.x, bounds.y, bounds.width, bounds.height + 4}, 0.1,
        2, ColorBlack
    );

    DrawRectangleRec(innerRect, ColorGrayLighter);
    DrawRectangleLinesEx(innerRect, thick, ColorWhite);

    Color sc = ColorBlack;

    DrawRectangleRec(
        (Rectangle){
            innerRect.x,
            innerRect.y,
            thick,
            thick,
        },
        sc
    );

    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick, innerRect.y, thick,
                    thick},
        sc
    );

    DrawRectangleRec(
        (Rectangle){
            innerRect.x,
            innerRect.y + innerRect.height - thick,
            thick,
            thick,
        },
        sc
    );

    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick,
                    innerRect.y + innerRect.height - thick, thick, thick},
        sc
    );

    // GuiDrawIcon(ICON_GRID_FILL, innerRect.x + 5, innerRect.y + 5, 1,
    // ColorWhite);

    return clicked;
}
