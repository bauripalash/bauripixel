#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include <stdbool.h>

#define SHADOW_SIZE 3

void BpRoundedPanel(Rectangle bounds, float thick, float shadowr, bool shadow) {
    BpDummyPanel(bounds, thick, shadowr, shadow);
}

void BpRoundedFlatPanel(Rectangle bounds, float roundness) {

    DrawRectangleRounded(bounds, roundness, 0, ColorGrayDarker);
    DrawRectangleRoundedLinesEx(bounds, roundness, 0, 2, ColorGrayLightest);
}

void BpMenuBarPanel(Vector2 position, float width, int items, float roundness) {
    Font font = GuiGetFont();
    float panelHeight = font.baseSize * items;

    Rectangle rect = {position.x, position.y, width, panelHeight};

    BpRoundedFlatPanel(rect, roundness);
}

void BpDummyPanel(Rectangle bounds, float thick, float roundness, bool shadow) {

    Rectangle innerRect = {
        bounds.x + thick, bounds.y + thick, bounds.width - thick * 2,
        bounds.height - thick * 2
    };

    int shadowHeight = bounds.height;
    if (shadow) {
        shadowHeight += 4;
    }

    DrawRectangleRounded(
        (Rectangle){bounds.x, bounds.y, bounds.width, bounds.height + 4},
        shadow ? 0.07 : roundness, 2, ColorBlack
    );

    int tk = 4;
    Rectangle faceFg = {
        innerRect.x + tk, innerRect.y + tk, innerRect.width - tk * 2,
        innerRect.height - tk * 2
    };

    DrawRectangleRec(innerRect, Fade(ColorXGrayLight, 0.3));
    DrawRectangleLinesEx(innerRect, thick, ColorXWhite);
    /*
    Vector2 tl = {innerRect.x, innerRect.y};
    Vector2 tr = {innerRect.x + innerRect.width, innerRect.y};
    Vector2 bl = {innerRect.x, innerRect.y + innerRect.height};
    Vector2 br = {innerRect.x + innerRect.width, innerRect.y +
    innerRect.height};


    DrawLineEx(tl, tr, thick, Fade(ColorXWhite, 0.5));
    DrawLineEx(tl, bl, thick, Fade(ColorXWhite,0.5));
    DrawLineEx(bl, br, thick, Fade(ColorXWhite,0.5));
    DrawLineEx(tr, br, thick, Fade(ColorXWhite,0.5));
    */
    Color sc = ColorXGrayLight;

    DrawRectangleRec(
        (Rectangle){
            innerRect.x,
            innerRect.y,
            thick,
            thick,
        },
        sc
    );

    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick, innerRect.y, thick,
                    thick},
        sc
    );

    DrawRectangleRec(
        (Rectangle){
            innerRect.x,
            innerRect.y + innerRect.height - thick,
            thick,
            thick,
        },
        sc
    );

    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick,
                    innerRect.y + innerRect.height - thick, thick, thick},
        sc
    );
}
