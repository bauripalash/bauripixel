/*
 * drawarea.c - Drawing Area Widget
 * Copyright (C) <year>  <name of author>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../colors.h"
#include "../components.h"
#include "../external/raylib/raylib.h"
#include "../widget.h"
#include "raylib.h"

int drawAreaDraw(BpWidget *base, double dt, void *ctx) {
    if (!base->enabled || !base->visible) {
        return 1;
    }

    BpDrawArea *da = (BpDrawArea *)base;
    BpPanelBorder(base->bounds, 2);
    DrawRectangleLinesEx(da->usableRect, 2, BpColorBlack);

    return 0;
}

int drawAreaUpdate(BpWidget *base, double dt, void *ctx) {
    if (!base->enabled) {
        return 1;
    }
    BpDrawArea *da = (BpDrawArea *)base;
    Rectangle bounds = base->bounds;
    Rectangle usableRect = {
        bounds.x + DRWAREA_PADDING, bounds.y + DRWAREA_PADDING,
        bounds.width - DRWAREA_PADDING * 2, bounds.height - DRWAREA_PADDING * 2
    };

    da->usableRect = usableRect;

    return 0;
}

BpDrawArea NewDrawArea(void) {
    BpDrawArea da = {0};
    da.w = (BpWidget){
        .enabled = true,
        .visible = true,
        .bounds = (Rectangle){10, 10, 400, 400},
        .anchor = (Vector2){0},
        .stopper = (Vector2){0},
        .Update = drawAreaUpdate,
        .Draw = drawAreaDraw
    };
    da.color = BpColorBlack;
    // da.bgImg = GenImageChecked(, int height, int checksX, int checksY, Color
    // col1, Color col2)
    return da;
}
