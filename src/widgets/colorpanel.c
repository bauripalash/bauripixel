#include "../external/raylib/raygui.h"
#include "../external/raylib/raylib.h"
#include "../external/stb/stb_ds.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/utils.h"
#include "../include/widget.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define CP_MARGIN  5
#define CP_PADDING 5

static void updateBounds(BpColorPanel *panel) {}

int colorPanelDraw(BpWidget *base, double dt, void *ctx) {
    BpPanelBorder(base->bounds, 5);
    BpColorPanel *cp = (BpColorPanel *)base;

    Rectangle usableRect = cp->usableRect;
    int maxColumns = cp->maxColumns;
    int colorCount = cp->colorCount;
    int boxSize = cp->boxSize;

    BeginScissorModeRec(usableRect);
    for (int ci = 0; ci < colorCount; ci++) {
        // Calculate in row,column this `ci`th indexed color box will go
        int cboxColumn = ci % maxColumns;
        int cboxRow = ci / maxColumns;

        Color cboxBorderColor =
            cp->curColorIndex == ci ? BpColorWhite : BpColorBlack;

        Rectangle colorRect = {
            usableRect.x + cp->scroll.x + (cboxColumn * boxSize),
            usableRect.y + cp->scroll.y + (cboxRow * boxSize), boxSize, boxSize
        };

        DrawRectangleRec(colorRect, cp->colors[ci]);
        DrawRectangleLinesEx(colorRect, 2, cboxBorderColor);
    }
    EndScissorMode();

    int ogDefBg = GuiGetStyle(DEFAULT, BACKGROUND_COLOR);
    int ogLvBorderW = GuiGetStyle(LISTVIEW, BORDER_WIDTH);
    int ogBtnBase = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, BpHexColorTransparent);
    GuiSetStyle(LISTVIEW, BORDER_WIDTH, 0);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, BpHexColorVGray);

    GuiScrollPanel(cp->usableRect, NULL, cp->usedRect, &cp->scroll, &cp->view);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ogDefBg);
    GuiSetStyle(LISTVIEW, BORDER_WIDTH, ogLvBorderW);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ogBtnBase);
    // DrawRectangleLinesEx(usableRect, 2, RED);
    // DrawRectangleLinesEx(usedRect, 2, GREEN);

    return 0;
}

int colorPanelUpdate(BpWidget *base, double dt, void *ctx) {
    updateBounds((BpColorPanel *)base);
    BpColorPanel *cp = (BpColorPanel *)base;
    bool locked = GuiIsLocked();

    Rectangle usableRect = (Rectangle){
        base->bounds.x + CP_PADDING,
        base->bounds.y + CP_PADDING,
        base->bounds.width - CP_PADDING * 2,
        base->bounds.height - CP_PADDING * 2,
    };
    int colorCount = cp->colorCount;
    int boxSize = cp->boxSize;

    // How many columns of color box can be shown in panel
    int maxColorColumns = (int)(floorf(usableRect.width / cp->boxSize));

    // We check how many columns will be required or will be used.
    // if we have less colors than max column that means, not even a single row
    // will be filled. so the column count will just be color count.
    int usedColumns =
        (colorCount < maxColorColumns) ? colorCount : maxColorColumns;

    // How many full rows it will take to for these many colored boxes
    // if there is remainder in this division it means there will be some color
    // boxes which will take an extra row but it will not be a filled row
    int usedRows = colorCount / maxColorColumns;

    // increase a row because some colored boxes are left
    // which are less to fill a full row
    if ((colorCount % maxColorColumns) > 0) {
        usedRows++;
    }

    // If we draw each color box according to its row,column place,
    // how much actual space will be used.
    Rectangle usedRect = {
        usableRect.x, usableRect.y, usedColumns * cp->boxSize,
        usedRows * cp->boxSize
    };

    for (int ci = 0; ci < colorCount; ci++) {
        // Calculate in row,column this `ci`th indexed color box will go
        int cboxColumn = ci % maxColorColumns;
        int cboxRow = ci / maxColorColumns;

        Rectangle colorRect = {
            usableRect.x + cp->scroll.x + (cboxColumn * boxSize),
            usableRect.y + cp->scroll.y + (cboxRow * boxSize), boxSize, boxSize
        };
        Color curColor = cp->colors[ci];
        Vector2 mpos = GetMousePosition();

        if (CheckCollisionRecs(cp->view, colorRect) &&
            CheckCollisionPointRec(mpos, colorRect)) {
            cp->hoverColorIndex = ci;
            cp->hoverColor = curColor;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !locked) {
                cp->curColorIndex = ci;
                cp->curColor = curColor;
            }
        }
    }

    // Update states
    cp->usedRect = usedRect;
    cp->usableRect = usableRect;
    cp->maxColumns = maxColorColumns;
    cp->usedColumn = usedColumns;
    cp->usedRow = usedRows;
    return 0;
}

static void pushDefaultColors(BpColorPanel *panel) {
    for (int i = 0; i < 5; i++) {
        arrput(panel->colors, BpColorVGray);
        arrput(panel->colors, BpColorVGrayLight);
        arrput(panel->colors, BpColorVGreen);
        arrput(panel->colors, BpColorVWhite);

        arrput(panel->colors, BpColorCWhite);
        arrput(panel->colors, BpColorCBlack);
        arrput(panel->colors, BpColorCGray);
        arrput(panel->colors, BpColorCOrange);
    }
    panel->colorCount = arrlen(panel->colors);
}

BpColorPanel NewColorPanel(void) {
    BpColorPanel cp = {0};
    cp.w = (BpWidget){
        .bounds = (Rectangle){50, 50, 150, 300},
        .anchor = (Vector2){0},
        .stopper = (Vector2){0},
        .enabled = true,
        .visible = true,
        .parent = NULL,
        .Update = colorPanelUpdate,
        .Draw = colorPanelDraw,
    };

    cp.w.bounds.x = cp.w.bounds.x + CP_MARGIN + cp.w.anchor.x;
    cp.w.bounds.y = cp.w.bounds.y + CP_MARGIN + cp.w.anchor.y;

    cp.curColorIndex = 0;
    cp.hoverColorIndex = 0;
    cp.curColor = BpColorBlack;
    cp.hoverColor = BpColorBlack;
    cp.boxSize = 30;
    pushDefaultColors(&cp);

    return cp;
}
