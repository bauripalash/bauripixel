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

#define DA_SCROLL_THICKNESS 10

bool drawAreaScrollbarDraw(BpWidget *base) {
    BpDrawArea *da = (BpDrawArea *)base;
    Rectangle usableRect = da->usableRect;
    Rectangle vScrollRect = {
        usableRect.x + usableRect.width,
        usableRect.y,
        DA_SCROLL_THICKNESS,
        usableRect.height,
    };

    Rectangle hScrollRect = {
        usableRect.x,
        usableRect.y + usableRect.height,
        usableRect.width,
        DA_SCROLL_THICKNESS,
    };

    DrawRectangleRec(vScrollRect, BpColorBlack);
    DrawRectangleRec(hScrollRect, BpColorBlack);
    return false;
}

bool drawAreaScrollbarUpdate(BpWidget *base) { return false; }

int drawAreaDraw(BpWidget *base, double dt, void *ctx) {
    if (!base->enabled || !base->visible) {
        return 1;
    }

    BpDrawArea *da = (BpDrawArea *)base;
    Rectangle usableRect = da->usableRect;
    BpPanelBorder(base->bounds, 2);
    DrawRectangleLinesEx(da->usableRect, 2, BpColorBlack);
    drawAreaScrollbarDraw(base);
    BeginScissorModeRec(usableRect);
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
    Rectangle canvasArea = {
        usableRect.x, usableRect.y, da->canvasWidth, da->canvasHeight
    };
    bool isHovering = CheckCollisionPointRec(mouse, usableRect) && !isLocked;
    da->hovering = isHovering;

    if (isHovering) {
        float mWheel = GetMouseWheelMove();
        if (mWheel != 0) {
            Vector2 mWorldPos = GetScreenToWorld2D(mouse, da->camera);
            da->camera.offset = mouse;
            da->camera.target = mWorldPos;
            da->point = mWorldPos;

            float scale = 0.2f * mWheel;
            da->camera.zoom =
                Clamp(expf(logf(da->camera.zoom) + scale), 0.125f, 64.0f);
        } // mWheel != 0

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
            (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))) {
            da->panning = true;
            Vector2 delta =
                Vector2Scale(GetMouseDelta(), -1.0 / da->camera.zoom);
            da->point = Vector2Add(da->point, delta);
            da->camera.target = da->point;
            TraceVector2("target ->", da->camera.target);
        } else {
            da->panning = false;
        }
        da->camera.target.x = da->point.x;
        da->camera.target.y = da->point.y;
    } // isHovering
    drawAreaScrollbarUpdate(base);

    return 0;
}

BpDrawArea NewDrawArea(int canvasW, int canvasH) {
    BpDrawArea da = {0};
    da.w = (BpWidget){
        .parent = NULL,
        .enabled = true,
        .visible = true,
        .bounds = (Rectangle){10, 10, 400, 400},
        .anchor = (Vector2){0},
        .stopper = (Vector2){0},
        .Update = drawAreaUpdate,
        .Draw = drawAreaDraw,
    };
    da.color = BpColorBlack;
    da.canvasWidth = canvasW;
    da.canvasHeight = canvasH;
    da.bgColor = BLANK;
    da.bgImg = GenImageChecked(
        canvasW, canvasH, 8, 8, BpColorCheckerLight, BpColorCheckerDark
    );
    da.bgTxt = LoadTextureFromImage(da.bgImg);

    da.camera = (Camera2D){
        .zoom = 1.0f,
        .offset = (Vector2){0},
        .target = (Vector2){0},
        .rotation = 0
    };
    da.point = (Vector2){0};
    da.hovering = false;
    da.panning = false;
    da.usableRect = (Rectangle){0};
    return da;
}
