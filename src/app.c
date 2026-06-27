#include "app.h"
#include "colors.h"
#include "defaults.h"
#include "external/raylib/raylib.h"
#include "external/raylib/src/raylib.h"
#include "theme.h"
#include "widget.h"
#include <stdbool.h>
#include <stdlib.h>

int LayoutUpdate(double dt);
int LayoutDraw(double dt);

BpColorPanel cp = {0};
BpDrawArea da = {0};

int RunApp(void) {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT, APP_DEFAULT_TITLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    LoadDefaultTheme();
    LoadDefaultAppFont();

    cp = NewColorPanel();
    cp.w.enabled = false;
    da = NewDrawArea();

    double dt = GetFrameTime();
    while (!WindowShouldClose()) {
        dt = GetFrameTime();
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        if (IsKeyPressed(KEY_F10)) {
            ToggleBorderlessWindowed();
        }

        LayoutUpdate(dt);
        BeginDrawing();
        {
            ClearBackground(BpColorAppBG);
            LayoutDraw(dt);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

int LayoutUpdate(double dt) {
    cp.w.Update(Widget(&cp), dt, NULL);
    da.w.Update(Widget(&da), dt, NULL);
    return 0;
}
int LayoutDraw(double dt) {
    cp.w.Draw(Widget(&cp), dt, NULL);
    da.w.Draw(Widget(&da), dt, NULL);
    return 0;
}
