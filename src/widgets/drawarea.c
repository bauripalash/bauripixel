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
#include "../external/raylib/raygui.h"
#include "../external/raylib/raylib.h"
#include "../external/raylib/raymath.h"
#include "../utils.h"
#include "../widget.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>

int drawAreaDraw(BpWidget *base, double dt, void *ctx) {
    if (!base->enabled || !base->visible) {
        return 1;
    }

    BpDrawArea *da = (BpDrawArea *)base;
    Rectangle usableRect = da->usableRect;
    BpPanelBorder(base->bounds, 2);
    DrawRectangleLinesEx(da->usableRect, 2, BpColorBlack);
    BeginScissorModeRec(usableRect);
    GuiGrid(usableRect, NULL, da->canvasWidth * 2.0f, 1, NULL);
    BeginMode2D(da->camera);
    {
        DrawTexture(da->bgTxt, usableRect.x, usableRect.y, BpColorWhite);
    }
    EndMode2D();
    EndScissorMode();

    return 0;
}

int drawAreaUpdate(BpWidget *base, double dt, void *ctx) {
    if (!base->enabled) {
        return 1;
    }
    bool isLocked = GuiIsLocked();
    Vector2 mouse = GetMousePosition();
    BpDrawArea *da = (BpDrawArea *)base;
    Rectangle bounds = base->bounds;
    Rectangle usableRect = {
        bounds.x + DRWAREA_PADDING, bounds.y + DRWAREA_PADDING,
        bounds.width - DRWAREA_PADDING * 2, bounds.height - DRWAREA_PADDING * 2
    };

    da->usableRect = usableRect;
    bool isHovering = CheckCollisionPointRec(mouse, usableRect) && !isLocked;
    da->hovering = isHovering;

    if (isHovering) {
        float mWheel = GetMouseWheelMove();
        if (mWheel != 0) {
            Vector2 mWorldPos = GetScreenToWorld2D(mouse, da->camera);
            da->camera.offset = mouse;
            da->camera.target = mWorldPos;

            float scale = 0.2f * mWheel;
            da->camera.zoom =
                Clamp(expf(logf(da->camera.zoom) + scale), 0.125f, 64.0f);
        } // mWheel != 0
    } // isHovering

    return 0;
}

BpDrawArea NewDrawArea(int canvasW, int canvasH) {
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
    da.canvasWidth = canvasW;
    da.canvasHeight = canvasH;

    da.camera = (Camera2D){
        .zoom = 1.0f,
        .offset = (Vector2){0},
        .target = (Vector2){0},
        .rotation = 0
    };

    da.bgImg = GenImageChecked(
        canvasW, canvasH, 8, 8, BpColorCheckerLight, BpColorCheckerDark
    );
    da.bgTxt = LoadTextureFromImage(da.bgImg);
    return da;
}
