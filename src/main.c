#include <stdio.h>
#include "external/raylib/raylib.h"
#include "raylib.h"
int main(void){
    SetTraceLogLevel(LOG_WARNING);

    InitWindow(640, 480, "BauriPixel");
    // SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);
		DrawRectangleRec((Rectangle){0,0 ,50 , 50}, BLACK);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
