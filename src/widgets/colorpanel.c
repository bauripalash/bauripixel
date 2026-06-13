#include "../external/raylib/raylib.h"
#include "../external/stb/stb_ds.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/widget.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>

#define CP_MARGIN  5
#define CP_PADDING 5

static void updateBounds(BpColorPanel *panel) {}

int colorPanelDraw(BpWidget *base, double dt, void *ctx) {
    BpPanelBorder(base->bounds, 5);
    BpColorPanel *cp = (BpColorPanel *)base;

    Rectangle usableRect = cp->usableRect;
    Rectangle usedRect = cp->usedRect;
    int maxColumns = cp->maxColumns;
    int colorCount = cp->colorCount;
    int boxSize = cp->boxSize;

    for (int ci = 0; ci < colorCount; ci++) {
        // Calculate in row,column this `ci`th indexed color box will go
        int cboxColumn = ci % maxColumns;
        int cboxRow = ci / maxColumns;

        Rectangle colorRect = {
            usableRect.x + (cboxColumn * boxSize),
            usableRect.y + (cboxRow * boxSize), boxSize, boxSize
        };

        DrawRectangleRec(colorRect, cp->colors[ci]);
    }

    DrawRectangleLinesEx(usableRect, 2, RED);
    DrawRectangleLinesEx(usedRect, 2, GREEN);

    return 0;
}

int colorPanelUpdate(BpWidget *base, double dt, void *ctx) {
    updateBounds((BpColorPanel *)base);
    BpColorPanel *cp = (BpColorPanel *)base;

    Rectangle usableRect = (Rectangle){
        base->bounds.x + CP_PADDING,
        base->bounds.y + CP_PADDING,
        base->bounds.width - CP_PADDING * 2,
        base->bounds.height - CP_PADDING * 2,
    };
    int colorCount = cp->colorCount;

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

    cp.colorIndex = 0;
    cp.boxSize = 30;
    pushDefaultColors(&cp);

    return cp;
}
