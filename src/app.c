#include "external/raylib.h"
#include "include/widgets/colorbar.h"
#include "include/widgets/widget.h"
#include <stdbool.h>

#define RAYGUI_GRID_ALPHA 0.7f
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"
#define STB_DS_IMPLEMENTATION
#include "external/stb/stb_ds.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 700
int WinWidth = WINDOW_WIDTH;
int WinHeight = WINDOW_HEIGHT;
Vector2 mousePos = {0, 0};
#define GridCol 16
#define GridRow 16
int CellSize = 40;

Color colorCanvas[GridRow][GridCol];
ColorBarState cb;

void Layout();
void ApplyStyle();

int RunApp() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BauriPixel");
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(60);

    cb = NewColorBar();
    for (int i = 0; i < 8; i++) {
        AddToColorBar(&cb, RED);
        AddToColorBar(&cb, GREEN);
        AddToColorBar(&cb, BLUE);
        AddToColorBar(&cb, PINK);
        AddToColorBar(&cb, MAGENTA);
    }
    SetColorBarAnchor(&cb, (Vector2){0, 50}, (Vector2){-1, 50});
    cb.prop.active = true;

    for (int row = 0; row < GridRow; row++) {
        for (int col = 0; col < GridCol; col++) {
            colorCanvas[row][col] = WHITE;
        }
    }

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 5);

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

    ClearColorBar(&cb);
    CloseWindow();

    return 0;
}

Color currentColor = MAGENTA;
Color prevColor = RAYWHITE;

void Layout() {
    ColorBar(&cb);

    if (!ColorIsEqual(cb.currentColor, RAYWHITE)) {
        if (!ColorIsEqual(cb.currentColor, prevColor)) {
            TraceLog(
                LOG_WARNING, "Selected -> %d", ColorToInt(cb.currentColor)
            );
            prevColor = cb.currentColor;
        }
    }

    /*
    if (IsKeyPressed(KEY_G)) {
            currentColor = GREEN;
    }else if (IsKeyPressed(KEY_R)){
            currentColor = RED;
    }else if (IsKeyPressed(KEY_W)){
            currentColor = WHITE;
    }
    for (int row = 0; row < GridRow; row++) {
            for (int col = 0; col < GridCol; col++) {
                    Rectangle btnRect = {col * CellSize, row * CellSize,
    CellSize, CellSize}; DrawRectangleRec(btnRect, colorCanvas[row][col]);
                    //DrawRectangleLinesEx(btnRect, 1 , BLACK);

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
    CheckCollisionPointRec(mousePos, btnRect)) { colorCanvas[row][col] =
    currentColor;
                    }
            }
    }

    Rectangle gridRect = {0,0 , GridCol * CellSize, GridRow * CellSize};
    //GuiGrid(gridRect, NULL, CellSize, 1, NULL);
    */
}
