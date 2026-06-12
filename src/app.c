#include "include/app.h"
#include "external/raylib/raylib.h"
#include "external/raylib/src/raylib.h"
#include "include/colors.h"
#include "include/components.h"
#include "include/defaults.h"
#include "include/theme.h"
#include "include/widget.h"
#include <stdlib.h>

int LayoutUpdate(double dt);
int LayoutDraw(double dt);

BpColorPanel cp = {0};

int RunApp(void) {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT, APP_DEFAULT_TITLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    LoadDefaultTheme();
    LoadDefaultAppFont();

    cp = NewColorPanel();

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
            ClearBackground(BpColorCBlack);
            LayoutDraw(dt);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

int LayoutUpdate(double dt) {
    cp.w.Update(Widget(&cp), dt, NULL);
    return 0;
}
int LayoutDraw(double dt) {
    cp.w.Draw(Widget(&cp), dt, NULL);
    return 0;
}
