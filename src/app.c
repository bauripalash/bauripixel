#include "include/app.h"
#include "external/raylib/raylib.h"
#include "external/raylib/src/raylib.h"
#include "include/colors.h"
#include "include/defaults.h"
#include "include/widget.h"
#include "include/wpanel.h"
#include "raymath.h"
#include <stdlib.h>

int LayoutUpdate(double dt);
int LayoutDraw(double dt);

WPanel panel = {0};

int RunApp(void) {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(APP_DEFAULT_WIDTH, APP_DEFAULT_HEIGHT, APP_DEFAULT_TITLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    panel = NewWPanel(100, 100);
    PanelSetColor(&panel, BpColorCBlack, BpColorCOrange);
    PanelSetAnchor(&panel, (Vector2){50, 50}, Vector2Zero());

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
    panel.w.Update(Widget(&panel), dt, NULL);
    return 0;
}
int LayoutDraw(double dt) {
    panel.w.Draw(Widget(&panel), dt, NULL);
    return 0;
}
