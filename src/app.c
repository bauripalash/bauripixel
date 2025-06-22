#include "external/raylib.h"
#include <string.h>
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"


#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 700
int WinWidth = WINDOW_WIDTH;
int WinHeight = WINDOW_HEIGHT;
Vector2 mousePos = {0, 0};
#define GridCol 16
#define GridRow 16
int CellSize = 40 ;

Color colorCanvas[GridRow][GridCol];

void Layout();
void ApplyStyle();

int RunApp() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BauriPixel");
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(60);
    int fontSize = 10;

	for (int row = 0; row < GridRow; row++) {
		for (int col = 0; col < GridCol; col++) {
			colorCanvas[row][col] = WHITE;
		}
	}

    while (!WindowShouldClose()) {
        WinWidth = GetScreenWidth();
        WinHeight = GetScreenHeight();
        mousePos = GetMousePosition();


        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
			Layout();

        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

Color currentColor = MAGENTA;

void Layout() {

	if (IsKeyPressed(KEY_G)) {
		currentColor = GREEN;
	}else if (IsKeyPressed(KEY_R)){
		currentColor = RED;
	}else if (IsKeyPressed(KEY_W)){
		currentColor = WHITE;
	}
	for (int row = 0; row < GridRow; row++) {
		for (int col = 0; col < GridCol; col++) {
			Rectangle btnRect = {col * CellSize, row * CellSize, CellSize, CellSize};
			DrawRectangleRec(btnRect, colorCanvas[row][col]);
			//DrawRectangleLinesEx(btnRect, 1 , BLACK);

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, btnRect)) {
				colorCanvas[row][col] = currentColor;
			}
		}
	}

	Rectangle gridRect = {0,0 , GridCol * CellSize, GridRow * CellSize};
	GuiGrid(gridRect, NULL, CellSize, 2, NULL);
}
