#include "../include/widgets/canvas.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

static Vector2 sPoint = {0, 0};
CanvasState NewCanvas() {
    CanvasState c = {0};
    c.prop = NewWidgetProp();
    c.hoverX = 0;
    c.hoverY = 0;
    c.gridSize = (Vector2){DEFAULT_GRID_SIZE, DEFAULT_GRID_SIZE};

    c.scroll = (Vector2){0, 0};
    c.zoomMin = 0.2f;
    c.zoomMax = INIT_CELL_SIZE;

    for (int i = 0; i < (int)c.gridSize.y; i++) {
        for (int j = 0; j < (int)c.gridSize.x; j++) {
            c.colors[i][j] = ColorWhite;
        }
    }

    c.colors[0][0] = ColorBlack;

    c.pxSize = INIT_CELL_SIZE;
    c.prop.bounds.x = CANVAS_MARGIN_L;
    c.prop.bounds.y = CANVAS_MARGIN_TB;
    c.prop.bounds.width =
        GetScreenWidth() - (CANVAS_MARGIN_L + CANVAS_MARGIN_R);
    c.prop.bounds.height = GetScreenHeight() - (CANVAS_MARGIN_TB * 2);

    c.camera = (Camera2D){0};
    c.camera.zoom = 1.0f;

    Rectangle bounds = c.prop.bounds;
    c.drawArea = (Rectangle){bounds.x + CANVAS_DRAW_MARGIN,
                             bounds.y + CANVAS_DRAW_MARGIN,
                             bounds.width - (CANVAS_DRAW_MARGIN * 2),
                             bounds.height - (CANVAS_DRAW_MARGIN * 2)};

    // c.camera.target = (Vector2){c.drawArea.width / 2.0f, c.drawArea.height
    // / 2.0f};

    // sPoint.x = c.drawArea.width / 2.0f;
    // sPoint.y = c.drawArea.height / 2.0f;

    return c;
}

void updateBounds(CanvasState *c) {
    c->prop.bounds.x = CANVAS_MARGIN_L;
    c->prop.bounds.y = CANVAS_MARGIN_TB;
    c->prop.bounds.width =
        GetScreenWidth() - (CANVAS_MARGIN_L + CANVAS_MARGIN_R);
    c->prop.bounds.height = GetScreenHeight() - (CANVAS_MARGIN_TB * 2);

    Rectangle bounds = c->prop.bounds;
    c->drawArea = (Rectangle){bounds.x + CANVAS_DRAW_MARGIN,
                              bounds.y + CANVAS_DRAW_MARGIN,
                              bounds.width - (CANVAS_DRAW_MARGIN * 2),
                              bounds.height - (CANVAS_DRAW_MARGIN * 2)};
}

void TraceVector(Vector2 vec, const char *string) {
    TraceLog(LOG_WARNING, "%s [%f, %f]", string, vec.x, vec.y);
}

void TraceRect(Rectangle rect, const char *string) {
    TraceLog(
        LOG_WARNING, "%s [[%f, %f],[%f, %f]]", string, rect.x, rect.y,
        rect.width, rect.height
    );
}

void drawBorder(CanvasState *state) {
    DrawRectangleRec(state->drawArea, ColorWhite);
    DrawRectangleLinesEx(state->drawArea, 1, ColorGrayLighter);
}

#define TMR 3
#define TMW 50
static bool dragging = false;
static float displ = 0.0f;
bool BauScroll(
    Rectangle bounds, Vector2 *s, float move, float left, float right,
    float width
) {
    DrawRectangleRec(bounds, ColorGreenLightest);
    DrawRectangleLinesEx(bounds, 1, ColorOrange);
    float totalWidth = width;

    if (left <= 0)
        totalWidth += fabs(left);
    if (right <= 0)
        totalWidth += fabs(right);

    TraceLog(
        LOG_ERROR, "LEFT %f | RIGHT %f | TOTAL %f", left, right, totalWidth
    );
    float thumbX = bounds.x + displ;
    float thumbWidth = (512 / totalWidth) * bounds.width;
    Rectangle thumbRect = {
        thumbX, bounds.y + TMR, thumbWidth, bounds.height - (TMR * 2)
    };

    DrawRectangleRec(thumbRect, ColorGreenDarkest);
    // DrawRectangleLinesEx(thumbRect,2, ColorRedDark);
    float oldDisp = displ;
    float oldX = bounds.x + displ;

    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            dragging = true;
        }

        float wheel = GetMouseWheelMove();

        if (wheel != 0.0f) {
            float scale = 10 * wheel;
            displ += scale;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        dragging = false;
    }

    if (dragging) {
        Vector2 delta = GetMouseDelta();
        displ += delta.x;
    }

    if (displ <= 0) {
        displ = 0;
    }

    if (displ > (bounds.width - thumbWidth)) {
        displ = bounds.width - thumbWidth;
    }

    TraceLog(
        LOG_WARNING, "DISP -> %f | BOUNDS : %f | MAXDISP -> %f", displ,
        bounds.width, bounds.width - thumbWidth
    );

    s->x += displ - oldDisp;

    return false;
}

static float svalue = 0.0f;
static float oldsvalue = 0.0f;
static int sliderValue = 0;
static bool panning = false;
bool Canvas(CanvasState *state) {
    TraceLog(LOG_WARNING, "--------------------");
    updateBounds(state);
    Vector2 mpos = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mpos, state->drawArea);
    Rectangle deadZone;
    // EndDrawing();

    Vector2 drawVector = (Vector2){state->drawArea.x, state->drawArea.y};
    Rectangle canvasRect = {
        state->drawArea.x, state->drawArea.y,
        (state->gridSize.x * state->pxSize), (state->gridSize.y * state->pxSize)
    };

    Vector2 canvasVector = {canvasRect.x, canvasRect.y};

    if (isHovering) {
        float wheel = GetMouseWheelMove();

        if (wheel != 0) {
            Vector2 mwPos =
                GetScreenToWorld2D(GetMousePosition(), state->camera);
            state->camera.offset = GetMousePosition();
            state->camera.target = mwPos;
            sPoint = mwPos;
            float scale = 0.2f * wheel;
            state->camera.zoom += scale;
            state->camera.zoom = Clamp(state->camera.zoom, 0.2f, 10.0f);

            Vector2 center = {
                (state->drawArea.x + state->drawArea.width) / 2.0f,
                (state->drawArea.y + state->drawArea.height) / 2.0f
            };
            center = GetScreenToWorld2D(center, state->camera);
        }

        if (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

            Vector2 delta =
                Vector2Scale(GetMouseDelta(), -1.0 / state->camera.zoom);

            sPoint = Vector2Add(sPoint, delta);
            state->camera.target = sPoint;
            // state->camera.target = Vector2Add(state->camera.target, delta);

            panning = true;
        }
    }

    if (IsKeyReleased(KEY_LEFT_SHIFT) ||
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        panning = false;
    }

    TraceLog(LOG_ERROR, "Panning -> %s", panning ? "true" : "false");

    Vector2 dir = {0, 0};

    if (IsKeyDown(KEY_LEFT))
        dir.x++;
    if (IsKeyDown(KEY_RIGHT))
        dir.x--;
    if (IsKeyDown(KEY_UP))
        dir.y++;
    if (IsKeyDown(KEY_DOWN))
        dir.y--;

    bool movingKb = false;
    if (Vector2Length(dir) != 0.0f) {
        movingKb = true;

        dir = Vector2Scale(Vector2Normalize(dir), 200.0f * GetFrameTime());
        sPoint = Vector2Add(sPoint, dir);
    }

    Vector2 dTl = GetScreenToWorld2D(drawVector, state->camera);
    Vector2 dBr = GetScreenToWorld2D(
        (Vector2){drawVector.x + state->drawArea.width,
                  state->drawArea.y + state->drawArea.height},
        state->camera
    );

    Vector2 cTl = canvasVector;
    Vector2 cBr = {
        canvasVector.x + canvasRect.width, canvasVector.y + canvasRect.height
    };

    float leftEmpty = cTl.x - dTl.x;
    float rightEmpty = dBr.x - cBr.x;

    float topEmpty = cTl.y - dTl.y;
    float bottomEmpty = dBr.y - cBr.y;

    Rectangle sRect = {
        state->drawArea.x, state->drawArea.y + state->drawArea.height,
        state->drawArea.width, 20
    };
    BauScroll(
        sRect, &sPoint, 200.0f * GetFrameTime(), leftEmpty, rightEmpty,
        dBr.x - dTl.x
    );

    if (panning || movingKb) {
        float leftOutside = (canvasRect.x + canvasRect.width) - dTl.x;
        float rightOutside = canvasRect.x - dBr.x;
        float topOutside = (canvasRect.y + canvasRect.height) - dTl.y;
        float bottomOutside = canvasRect.y - dBr.y;
        if (leftOutside < 0) {
            sPoint.x += leftOutside;
        } else if (topOutside < 0) {
            sPoint.y += topOutside;
        } else if (rightOutside > 0) {
            sPoint.x += rightOutside;
        } else if (bottomOutside > 0) {
            sPoint.y += bottomOutside;
        }
    }

    drawBorder(state);
    BeginScissorMode(
        state->drawArea.x, state->drawArea.y, state->drawArea.width,
        state->drawArea.height
    );
    {
        BeginMode2D(state->camera);
        {
            DrawRectangleRec(canvasRect, ColorWhite);
            GuiGrid(canvasRect, NULL, (state)->gridSize.x, 1, NULL);
            DrawCircleV(dTl, 30, ColorRedDark);
            DrawCircleV(cTl, 30, ColorYellow);
            DrawLineEx(dTl, cTl, 3, ColorOrange);
            DrawCircleV(dBr, 30, ColorGreenLighter);
            DrawCircleV(cBr, 30, ColorBlueLighter);
            // DrawCircleV(state->camera.target, 30, PINK);
            // DrawText("T", state->camera.target.x, state->camera.target.y, 30,
            // BLACK); DrawCircleV(state->camera.offset, 30, PURPLE);

            DrawLineEx(dBr, cBr, 3, ColorOrange);

            DrawRectangleLinesEx(canvasRect, 2, ColorRedDark);

            // DrawCircleV(sPoint, 20, GREEN);

            // DrawRectangleLinesEx(deadZone, 2, ColorRedDark);
        }
        EndMode2D();
    }
    EndScissorMode();

    TraceLog(
        LOG_WARNING, "VIEWPORT [%f][%f]", dBr.x - dTl.x,
        (cBr.x - cTl.x) * state->camera.zoom
    );
    // BauScroll(sRect, &sPoint, move, leftEmpty, rightEmpty, dBr.x - dTl.x);

    state->camera.target.x = sPoint.x;
    state->camera.target.y = sPoint.y;
    TraceLog(LOG_WARNING, "--------------------");

    return false;
}
