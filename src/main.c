#include <stdbool.h>
// #define EXAMPLE

#ifndef EXAMPLE

#include "include/app.h"

int main(int argc, char **argv) {
    RunApp();
    return 0;
}

#else

#include "external/raylib.h"
#include "external/raymath.h"

#define WINDOW_WIDTH     800
#define WINDOW_HEIGHT    700
#define VP_MARGIN_L      100
#define VP_MARGIN_R      30
#define VP_MARGIN_T      30
#define VP_MARGIN_B      50
#define CANVAS_WIDTH     200
#define CANVAS_HEIGHT    200
#define SCROLL_THICKNESS 20

void HScrollBar(
    Rectangle bounds, bool *dragging, Vector2 *scroll, Vector2 *plr,
    Vector2 viewportTL, Vector2 viewportBR, Vector2 mapTL, Vector2 mapBR
) {
    float roundness = 1.0f;
    // DrawRectangleRec(bounds, LIGHTGRAY);
    DrawRectangleRounded(bounds, roundness, 0, LIGHTGRAY);

    float viewWidth = viewportBR.x - viewportTL.x;
    float mapWidth = mapBR.x - mapTL.x;
    float totalWidth = viewWidth + mapWidth * 2;

    float scale = totalWidth / bounds.width;
    float minPosX = -mapWidth;
    float maxPosX = viewWidth;

    scroll->x = (viewportBR.x - mapTL.x) / scale;

    float thumbWidth = (mapWidth / totalWidth) * bounds.width;
    Rectangle thumbRect = {
        bounds.x + scroll->x,
        bounds.y,
        thumbWidth,
        bounds.height,
    };

    float oldScroll = scroll->x;

    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            *dragging = true;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        *dragging = false;
    }

    if (*dragging) {
        Vector2 delta = GetMouseDelta();
        scroll->x += delta.x;
    }

    DrawRectangleRounded(thumbRect, roundness, 0, GRAY);
    DrawRectangleRoundedLinesEx(bounds, roundness, 0, 2, GREEN);

    scroll->x = Clamp(scroll->x, 0, bounds.width - thumbWidth);

    plr->x += (scroll->x - oldScroll) * scale;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib camera pan scroll example");
    SetTargetFPS(60);

    Rectangle viewport = {
        VP_MARGIN_L, VP_MARGIN_T, WINDOW_WIDTH - (VP_MARGIN_L + VP_MARGIN_R),
        WINDOW_HEIGHT - (VP_MARGIN_T + VP_MARGIN_B)
    };

    Rectangle map = {viewport.x, viewport.y, CANVAS_WIDTH, CANVAS_HEIGHT};

    Camera2D cam = {0};
    cam.zoom = 1.0f;
    bool panning = false;
    Vector2 player = {
        0,
        //-((viewport.width + viewport.x) * 0.5f) + (map.width / 2.0f),
        0
    };
    cam.target = player;

    cam.offset = (Vector2){(viewport.width / 2.0f) - (map.width / 2.0f),
                           (viewport.height / 2.0f) - (map.height / 2.0f)};

    Vector2 scroll = {0, 0};
    bool dragScrollH = false;

    while (!WindowShouldClose()) {
        viewport.width = GetScreenWidth() - (VP_MARGIN_L + VP_MARGIN_R);
        viewport.height = GetScreenHeight() - (VP_MARGIN_T + VP_MARGIN_B);

        Rectangle hScrollRect = {
            viewport.x + 10,
            viewport.y + viewport.height + 10,
            viewport.width - 20,
            10,
        };

        bool hoverViewport =
            CheckCollisionPointRec(GetMousePosition(), viewport);

        if (hoverViewport) {
            float wheel = GetMouseWheelMove();

            if (wheel != 0.0f) {
                Vector2 mposWorld = GetScreenToWorld2D(GetMousePosition(), cam);
                cam.offset = GetMousePosition();
                cam.target = mposWorld;
                player = mposWorld;
                cam.zoom += 0.2f * wheel;
                cam.zoom = Clamp(cam.zoom, 0.2f, 10.0f);
            }

            if (IsKeyDown(KEY_LEFT_SHIFT) &&
                IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                panning = true;
            }
        }

        if (panning) {
            Vector2 delta = Vector2Scale(GetMouseDelta(), -1.0 / cam.zoom);

            player = Vector2Add(cam.target, delta);
            cam.target = player;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            panning = false;
        }

        Vector2 viewTopLeft =
            GetScreenToWorld2D((Vector2){viewport.x, viewport.y}, cam);

        Vector2 viewBottomRight = GetScreenToWorld2D(
            (Vector2){viewport.x + viewport.width,
                      viewport.y + viewport.height},
            cam
        );

        Vector2 mapTopLeft = {map.x, map.y};

        Vector2 mapBottomRight = {map.x + map.width, map.y + map.height};

        cam.target = player;

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawRectangleLinesEx(viewport, 2, GRAY);
            BeginScissorMode(
                viewport.x, viewport.y, viewport.width, viewport.height
            );

            BeginMode2D(cam);
            {
                DrawRectangleLinesEx(map, 5, GRAY);
                DrawCircleV(player, 10, GREEN);

                DrawCircleV(viewTopLeft, 20, RED);
                DrawCircleV(viewBottomRight, 20, BLUE);

                DrawCircleV(mapTopLeft, 20, YELLOW);
                DrawCircleV(mapBottomRight, 20, PINK);
            }
            EndMode2D();
            EndScissorMode();
            HScrollBar(
                hScrollRect, &dragScrollH, &scroll, &player, viewTopLeft,
                viewBottomRight, mapTopLeft, mapBottomRight
            );
        }
        DrawLineEx(
            (Vector2){viewport.x + viewport.width / 2.0f,

                      0},
            (Vector2){viewport.x + viewport.width / 2.0f, GetScreenHeight()}, 1,
            BLACK
        );

        DrawLineEx(
            (Vector2){0, viewport.y + viewport.height / 2.0f},
            (Vector2){GetScreenWidth(), viewport.y + viewport.height / 2.0f}, 1,
            BLACK
        );
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#endif
