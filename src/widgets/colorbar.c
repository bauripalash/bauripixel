#include "../include/widgets/colorbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include <stdbool.h>
#include <string.h>

ColorBarState NewColorBar() {
    ColorBarState cb = {0};
    cb.prop = NewWidgetProp();
    cb.colorCount = 1;
    cb.currentColor = RAYWHITE;
    cb.scroll = (Vector2){0, 0};

    return cb;
}

int active = 0;
int scrollIndex = 0;
Color colors[] = {RED,     GREEN,     BLUE,  MAGENTA, SKYBLUE,   BLACK,
                  GRAY,    LIGHTGRAY, PINK,  RED,     GREEN,     BLUE,
                  MAGENTA, SKYBLUE,   BLACK, GRAY,    LIGHTGRAY, PINK,
                  RED,     GREEN,     BLUE,  MAGENTA, SKYBLUE,   BLACK,
                  GRAY,    LIGHTGRAY, PINK,  RED,     GREEN,     BLUE,
                  MAGENTA, SKYBLUE,   BLACK, GRAY,    LIGHTGRAY, PINK};
#define NUM_COL     (sizeof(colors) / sizeof(Color))

#define BOX_SIZE    30
#define BOX_SPACING 5
#define TOP_MARGIN  BOX_SPACING
Rectangle content = {
    0, 0, BOX_SIZE, TOP_MARGIN + (int)NUM_COL *(BOX_SIZE + BOX_SPACING)
};
Rectangle view = {0};

#define PANEL_MARGIN 0
int selected = -1;

#define HANDLE_W 20

bool dragging = false;
#define MIN_W 50.0f

bool ColorBar(ColorBarState *state) {
    if (state->prop.active) {

        Rectangle bound = state->prop.bounds;
        Vector2 mpos = GetMousePosition();

        if (bound.height == -1) {
            bound.height = GetScreenHeight() - bound.y * 2;
        }

        Rectangle handle = {
            bound.x + (bound.width - HANDLE_W * 0.5), bound.y, HANDLE_W,
            bound.height
        };

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &
            CheckCollisionPointRec(mpos, handle)) {
            dragging = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = false;
        }

        if (dragging) {
            state->prop.bounds.width = mpos.x - bound.x;

            TraceLog(
                LOG_WARNING, "WIDTH -> %f | %s", bound.width,
                bound.width < MIN_W ? "true" : "false"
            );
            if (state->prop.bounds.width < MIN_W) {
                state->prop.bounds.width = MIN_W;
            }
        }

        DrawRectangleRec(state->prop.bounds, LIGHTGRAY);
        DrawRectangleLinesEx(state->prop.bounds, 2, BLACK);
        Rectangle sBound = {
            bound.x + PANEL_MARGIN, bound.y + PANEL_MARGIN,
            bound.width - (PANEL_MARGIN * 2), bound.height - (PANEL_MARGIN * 2)
        };
        if (CheckCollisionPointRec(mpos, handle) || dragging) {

            Color handleColor = dragging ? BLUE : MAGENTA;
            DrawRectangleRounded(handle, 10, 1, Fade(handleColor, 0.3f));
            DrawText(
                "|>", handle.x + (handle.width * 0.5f), handle.height * 0.5f,
                20, BLACK
            );
        }
        GuiScrollPanel(sBound, NULL, content, &state->scroll, &view);

        BeginScissorMode(sBound.x, sBound.y, sBound.width, sBound.height);

        for (int i = 0; i < NUM_COL; i++) {
            float y = sBound.y + TOP_MARGIN + i * (BOX_SIZE + BOX_SPACING) +
                      state->scroll.y;
            Rectangle box = {
                sBound.x + (sBound.width - 5 - BOX_SIZE) * 0.5f, y, BOX_SIZE,
                BOX_SIZE
            };

            DrawRectangleRec(box, colors[i]);

            DrawRectangleLinesEx(box, 2, LIGHTGRAY);
            if (selected == i) {
                DrawRectangleLinesEx(box, 3, BLACK);
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                CheckCollisionPointRec(GetMousePosition(), box)) {
                if (selected == i) {
                    selected = -1;
                    state->currentColor = RAYWHITE;
                } else {
                    selected = i;
                    state->currentColor = colors[i];
                }
            }
        }

        EndScissorMode();
    }

    return false;
}
