#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"

#define SHADOW_SIZE 3

void BpRoundedPanel(Rectangle bounds, float roundness) {

    DrawRectangleRounded(
        (Rectangle){
            bounds.x - SHADOW_SIZE,
            bounds.y - SHADOW_SIZE,
            bounds.width + SHADOW_SIZE * 2,
            bounds.height + SHADOW_SIZE * 2,
        },
        roundness, 0, Fade(ColorGrayLightest, 0.1)
    );
    DrawRectangleRounded(
        (Rectangle){bounds.x, bounds.y, bounds.width, bounds.height}, roundness,
        0, ColorGrayDarkest
    );
    DrawRectangleRoundedLinesEx(
        (Rectangle){bounds.x + 2, bounds.y + 2, bounds.width - 4,
                    bounds.height - 4},
        roundness, 0, 2, ColorGrayLightest
    );
}

void BpRoundedFlatPanel(Rectangle bounds, float roundness) {

    DrawRectangleRounded(bounds, roundness, 0, ColorGrayDarkest);
    DrawRectangleRoundedLinesEx(bounds, roundness, 0, 2, ColorGrayLightest);
}

void BpMenuBarPanel(Vector2 position, float width, int items, float roundness) {
    Font font = GuiGetFont();
    float panelHeight = font.baseSize * items;

    Rectangle rect = {position.x, position.y, width, panelHeight};

    BpRoundedFlatPanel(rect, roundness);
}
