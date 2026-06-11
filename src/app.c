#include "include/app.h"
#include "external/raylib/raylib.h"
#include "external/raylib/src/raylib.h"
#include "include/defaults.h"

int LayoutUpdate(double dt);
int LayoutDraw(double dt);

int RunApp() {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT, APP_DEFAULT_TITLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    double dt = GetFrameTime();
    while (!WindowShouldClose()) {
        dt = GetFrameTime();
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        LayoutUpdate(dt);
        BeginDrawing();
        {
            ClearBackground(WHITE);
            LayoutDraw(dt);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

int LayoutUpdate(double dt) { return 0; }
int LayoutDraw(double dt) { return 0; }
