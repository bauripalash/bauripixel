#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/drawtools.h"
#include <stdbool.h>

#define ICON_WIDTH 32

bool BpDummyToogleButton(Rectangle bounds, bool active) {
    Color bg = ColorGrayDarkest;
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
                   IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

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
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
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

    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
         IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) &&
        !CheckCollisionPointRec(GetMousePosition(), bounds)) {
        *showOther = false;
    }

    return clicked;
}
