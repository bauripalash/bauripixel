/*
 * app.c - Base Application Handler
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
