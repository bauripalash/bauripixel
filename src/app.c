#include "include/app.h"
#include "external/raylib/raylib.h"
#include "external/raylib/src/raylib.h"
#include "include/colors.h"
#include "include/components.h"
#include "include/defaults.h"
#include "include/theme.h"

int LayoutUpdate(double dt);
int LayoutDraw(double dt);

int RunApp(void) {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT, APP_DEFAULT_TITLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    LoadDefaultTheme();
    LoadDefaultAppFont();

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

int LayoutUpdate(double dt) { return 0; }
int LayoutDraw(double dt) {
    BpPanelBorder((Rectangle){0, 0, 50, 50}, 2);
    return 0;
}
