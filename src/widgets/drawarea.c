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
#include "../options.h"
#include "../utils.h"
#include "../widget.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define DA_SCROLL_THICKNESS 10
#define DA_SCROLL_ROUNDNESS 0.9

// Clamp the fake player point for panning limit of canvas
static Vector2 drawAreaClampPoint(BpDrawArea *da, Vector2 targetPoint);

static Vector2 drawAreaClampPoint(BpDrawArea *da, Vector2 targetPoint) {
    // Create a copy of DrawArea's camera so we dont't have to mutate the
    // real camera
    Camera2D tempCamera = da->camera;
    tempCamera.target = targetPoint;

    // Viewport (Usable Rect) corners in WorldSpace
    // It is tell if the the camera looked at `target`, what would be visible
    Vector2 viewportTopLeftWord = GetScreenToWorld2D(
        (Vector2){da->viewport.x, da->viewport.y}, tempCamera
    );

    Vector2 viewportBottomRightWorld = GetScreenToWorld2D(
        (Vector2){
            da->viewport.x + da->viewport.width,
            da->viewport.y + da->viewport.height
        },
        tempCamera
    );

    // Canvas's edges, the canvas is a static object inside the WorldSpace
    // where we look through the lense of the camera, at the viewport is limit
    // of the area we can see inside the world
    // The Canvas never moves, only the camera's viewport changes, so it might
    // confuse us as if the canvas is moving.
    //
    // So canvas edges in WorldSpace are fixed in a sense
    float canvasLeft = da->canvasRect.x;
    float canvasRight = da->canvasRect.x + da->canvasRect.width;
    float canvasTop = da->canvasRect.y;
    float canvasBottom = da->canvasRect.y + da->canvasRect.height;

    // The viewport edges in WorldSpace
    float viewportLeft = viewportTopLeftWord.x;
    float viewportRight = viewportBottomRightWorld.x;
    float viewportTop = viewportTopLeftWord.y;
    float viewportBottom = viewportBottomRightWorld.y;

    // Panning Limit
    // the canvas left edge cannot go right to viewport's right edge
    if (canvasLeft > viewportRight) targetPoint.x += canvasLeft - viewportRight;
    // the canvas right edge cannot go left to viewport's left edge
    if (canvasRight < viewportLeft) targetPoint.x += canvasRight - viewportLeft;
    // the canvas top edge cannot go below the viewport's bottom edge
    if (canvasTop > viewportBottom) targetPoint.y += canvasTop - viewportBottom;
    // the canvas bottom edge cannot go above the viewport's top edge
    if (canvasBottom < viewportTop) targetPoint.y += canvasBottom - viewportTop;

    return targetPoint;
}

bool drawAreaScrollbarDraw(BpWidget *base) {
    BpDrawArea *da = (BpDrawArea *)base;

    Color scrollBgH = GetColor(OptThemeGet(T_SCROLLBAR_BG));
    Color scrollBgV = GetColor(OptThemeGet(T_SCROLLBAR_BG));
    Color scrollFgH = GetColor(OptThemeGet(T_SCROLLBAR_FG));
    Color scrollFgV = GetColor(OptThemeGet(T_SCROLLBAR_FG));

    if (da->vThumbHover) {
        scrollFgV = GetColor(OptThemeGet(T_SCROLLBAR_HVR_FG));
    }

    if (da->hThumbHover) {
        scrollFgH = GetColor(OptThemeGet(T_SCROLLBAR_HVR_FG));
    }

    Rectangle viewport = da->viewport;

    Rectangle vScrollRect = da->vScrollRect;
    Rectangle hScrollRect = da->hScrollRect;
    Rectangle vThumbRect = da->vThumbRect;
    Rectangle hThumbRect = da->hThumbRect;

    DrawRectangleRec(vScrollRect, scrollBgV);
    DrawRectangleRec(hScrollRect, scrollBgH);
    DrawRectangleRounded(vThumbRect, DA_SCROLL_ROUNDNESS, 0, scrollFgV);
    DrawRectangleRounded(hThumbRect, DA_SCROLL_ROUNDNESS, 0, scrollFgH);

    return false;
}

bool drawAreaScrollbarUpdate(BpWidget *base) {
    BpDrawArea *da = (BpDrawArea *)base;
    Rectangle viewport = da->viewport;
    bool isLocked = GuiIsLocked();
    Vector2 mouse = GetMousePosition();
    float zoom = da->camera.zoom;

    bool hThumbHover = false;
    bool vThumbHover = false;

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

    float vThumbHeight = 20;
    float hThumbHeight = 20;

    Rectangle vThumbRect = {
        vScrollRect.x, vScrollRect.y, DA_SCROLL_THICKNESS, vThumbHeight
    };

    Rectangle hThumbRect = {
        hScrollRect.x, hScrollRect.y, hThumbHeight, DA_SCROLL_THICKNESS
    };

    if (!isLocked && CheckCollisionPointRec(mouse, vThumbRect)) {
        vThumbHover = true;
    }

    if (!isLocked && CheckCollisionPointRec(mouse, hThumbRect)) {
        hThumbHover = true;
    }

    float canvasLeft = da->canvasRect.x;
    float canvasTop = da->canvasRect.y;
    float canvasWidth = da->canvasRect.width;
    float canvasHeight = da->canvasRect.height;

    Vector2 viewTopLeft =
        GetScreenToWorld2D((Vector2){viewport.x, viewport.y}, da->camera);
    Vector2 viewBottomRight = GetScreenToWorld2D(
        (Vector2){viewport.x + viewport.width, viewport.y + viewport.height},
        da->camera
    );

    TraceVector2("View TL", viewTopLeft);
    TraceVector2("View BR", viewBottomRight);

    da->hScrollRect = hScrollRect;
    da->vScrollRect = vScrollRect;
    da->vThumbRect = vThumbRect;
    da->hThumbRect = hThumbRect;
    da->vThumbHover = vThumbHover;
    da->hThumbHover = hThumbHover;
    return false;
}

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
        Vector2 viewTopLeft =
            GetScreenToWorld2D((Vector2){viewport.x, viewport.y}, da->camera);
        Vector2 viewBottomRight = GetScreenToWorld2D(
            (Vector2){
                viewport.x + viewport.width, viewport.y + viewport.height
            },
            da->camera
        );
        DrawTexture(da->bgTxt, canvasArea.x, canvasArea.y, BpColorWhite);
        DrawCircleLinesV(da->camera.target, 5, RED);
        DrawCircleLinesV(da->camera.offset, 5, BLUE);
        DrawRectangleLinesEx(viewport, 2, GREEN);
        DrawCircleLinesV(viewTopLeft, 10, PINK);
        DrawCircleLinesV(viewBottomRight, 10, MAGENTA);
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

    da->viewport = viewport;

    bool isHovering = CheckCollisionPointRec(mouse, viewport) && !isLocked;
    da->hovering = isHovering;

    if (isHovering) {
        float mWheel = GetMouseWheelMove();

        if (mWheel != 0) {

            Vector2 mWorldPos = GetScreenToWorld2D(mouse, da->camera);

            da->camera.offset = mouse;
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

    } // isHovering

    da->point = drawAreaClampPoint(da, da->point);
    // Syncs the player to camera's view
    da->camera.target.x = da->point.x;
    da->camera.target.y = da->point.y;
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
    da.vScrollRect = (Rectangle){
        da.viewport.x + da.viewport.width,
        da.viewport.y,
        DA_SCROLL_THICKNESS,
        da.viewport.height,
    };

    da.hScrollRect = (Rectangle){
        da.viewport.x,
        da.viewport.y + da.viewport.height,
        da.viewport.width,
        DA_SCROLL_THICKNESS,
    };
    da.vThumbRect = (Rectangle){0};
    da.hThumbRect = (Rectangle){0};
    da.vThumbHover = false;
    da.hThumbHover = false;
    DrawAreaCenterCanvas(&da.w);
    return da;
}
