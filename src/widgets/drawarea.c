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
    Rectangle viewport = da->viewport;
    Rectangle vScrollRect = {
        viewport.x + viewport.width,
        viewport.y,
        DA_SCROLL_THICKNESS,
        viewport.height,
    };

    Rectangle hScrollRect = {
        viewport.x,
        viewport.y + viewport.height,
        viewport.width,
        DA_SCROLL_THICKNESS,
    };

    DrawRectangleRec(vScrollRect, BpColorBlack);
    DrawRectangleRec(hScrollRect, BpColorBlack);
    return false;
}

bool drawAreaScrollbarUpdate(BpWidget *base) { return false; }

void DrawAreaCenterCanvas(BpWidget *base) {
    BpDrawArea *da = (BpDrawArea *)base;

    Rectangle viewport = da->viewport;
    Rectangle canvasRect = da->canvasRect;
    Vector2 viewportCenter = {
        viewport.x + viewport.width * 0.5f,
        viewport.y + viewport.height * 0.5f,
    };
    Vector2 canvasRectCenter = {
        canvasRect.x + canvasRect.width * 0.5f,
        canvasRect.y + canvasRect.height * 0.5f
    };

    da->camera.target = canvasRectCenter;
    da->camera.offset = viewportCenter;
    da->point = canvasRectCenter;

    float zoom = viewport.width / da->canvasWidth / 2.0f;
    da->camera.zoom = zoom;
}

int drawAreaDraw(BpWidget *base, double dt, void *ctx) {
    if (!base->enabled || !base->visible) {
        return 1;
    }

    BpDrawArea *da = (BpDrawArea *)base;
    Rectangle viewport = da->viewport;
    Rectangle canvasArea = da->canvasRect;

    BpPanelBorder(base->bounds, 2);
    DrawRectangleLinesEx(da->viewport, 2, BpColorBlack);
    drawAreaScrollbarDraw(base);
    BeginScissorModeRec(viewport);
    BeginMode2D(da->camera);
    {
        DrawRectangleLinesEx(viewport, 2, BLUE);
        DrawTexture(da->bgTxt, canvasArea.x, canvasArea.y, BpColorWhite);
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
    Rectangle viewport = {
        bounds.x + DRWAREA_PADDING, bounds.y + DRWAREA_PADDING,
        bounds.width - DRWAREA_PADDING * 2, bounds.height - DRWAREA_PADDING * 2
    };
    Rectangle canvasArea = da->canvasRect;

    da->viewport = viewport;

    bool isHovering = CheckCollisionPointRec(mouse, viewport) && !isLocked;
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

        } else {

            da->panning = false;
        }
        // Convert viewport from screen space to inside the world of camera
        // space. The viewport is basically what the camera sees inside the
        // world in the plane
        Vector2 worldViewPortTopLeft =
            GetScreenToWorld2D((Vector2){viewport.x, viewport.y}, da->camera);
        Vector2 worldViewPortBottomRight = GetScreenToWorld2D(
            (Vector2){
                viewport.x + viewport.width, viewport.y + viewport.height
            },
            da->camera
        );

        // The canvas is fixed in the world space
        // we just move the viewport via the camera
        Vector2 canvasTopLeft = {canvasArea.x, canvasArea.y};
        Vector2 canvasBottomRight = {
            canvasArea.x + canvasArea.width, canvasArea.y + canvasArea.height
        };
        if (da->panning & !isLocked) {
            // Canvas edges in world space
            float canvasLeft = canvasTopLeft.x;
            float canvasRight = canvasBottomRight.x;
            float canvasTop = canvasTopLeft.y;
            float canvasBottom = canvasBottomRight.y;

            // Viewport edges in world space
            float viewLeft = worldViewPortTopLeft.x;
            float viewRight = worldViewPortBottomRight.x;
            float viewTop = worldViewPortTopLeft.y;
            float viewBottom = worldViewPortBottomRight.y;

            // Limiting?
        }

        da->camera.target.x = da->point.x;
        da->camera.target.y = da->point.y;

    } // isHovering

    drawAreaScrollbarUpdate(base);
    da->canvasRect = (Rectangle){
        da->viewport.x, da->viewport.y, da->canvasWidth, da->canvasHeight
    };

    return 0;
}

BpDrawArea NewDrawArea(int canvasW, int canvasH) {
    BpDrawArea da = {0};
    Rectangle bounds = {10, 10, 400, 400};
    da.w = (BpWidget){
        .parent = NULL,
        .enabled = true,
        .visible = true,
        .bounds = bounds,
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
    da.viewport = (Rectangle){
        bounds.x + DRWAREA_PADDING, bounds.y + DRWAREA_PADDING,
        bounds.width - DRWAREA_PADDING * 2, bounds.height - DRWAREA_PADDING * 2
    };
    da.canvasRect = (Rectangle){da.viewport.x, da.viewport.y, canvasW, canvasH};
    DrawAreaCenterCanvas(&da.w);
    return da;
}
