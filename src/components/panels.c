#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/options.h"
#include <stdbool.h>

#define SHADOW_SIZE 3

void BpDummyPanel(
    Rectangle bounds, float thick, float roundness, bool shadow, Vector4 borders
);

void BpRoundedPanel(Rectangle bounds, float thick, float shadowr, bool shadow) {
    BpDummyPanel(bounds, thick, shadowr, shadow, (Vector4){0, 0, 0, 0});
}

void BpRoundedPanelBorder(
    Rectangle bounds, float thick, float r, Vector4 borders
) {}

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

void BpDummyPanel(
    Rectangle bounds, float thick, float roundness, bool shadow, Vector4 borders
) {
    Color panelBorder = GetColor(OptThemeGet(T_PANEL_BORDER));
    Color panelBg = GetColor(OptThemeGet(T_PANEL_BG));
    Color panelShadow = GetColor(OptThemeGet(T_PANEL_SHADOW));

    Rectangle innerRect = {
        bounds.x + thick, bounds.y + thick, bounds.width - thick * 2,
        bounds.height - thick * 2
    };

    int shadowHeight = bounds.height;
    if (shadow) {
        shadowHeight += 5;
    }

    DrawRectangleRounded(
        (Rectangle){bounds.x, bounds.y, bounds.width, shadowHeight},
        shadow ? 0.07 : roundness, 2, panelShadow
    );

    int tk = 4;
    Rectangle faceFg = {
        innerRect.x + tk, innerRect.y + tk, innerRect.width - tk * 2,
        innerRect.height - tk * 2
    };

    DrawRectangleRec(
        innerRect, Fade(panelBg, 0.1)
    ); // Fade(ColorVGrayLight, 0.3));
    // DrawRectangleLinesEx(innerRect, thick, panelBorder);

    Vector2 tl = {innerRect.x + thick, innerRect.y + thick};
    Vector2 tr = {
        innerRect.x + innerRect.width - thick * 2, innerRect.y + thick
    };
    Vector2 bl = {innerRect.x + thick, innerRect.y + innerRect.height - thick};
    Vector2 br = {
        innerRect.x + innerRect.width - thick,
        innerRect.y + innerRect.height - thick
    };

    Color sc = ColorVGrayLight;

    if (borders.x != -1) {
        Color brdr = panelBorder;
        if (borders.x != 0) {
            brdr = GetColor((int)borders.x);
        }
        DrawRectangleRec(
            (Rectangle){innerRect.x, innerRect.y, thick, innerRect.height}, brdr
        );

        // DrawLineEx(tl, bl, thick, brdr);
    }

    if (borders.y != -1) {
        Color brdr = panelBorder;
        if (borders.y != 0) {
            brdr = GetColor((int)borders.y);
        }

        DrawRectangleRec(
            (Rectangle){innerRect.x + innerRect.width - thick, innerRect.y,
                        thick, innerRect.height},
            brdr
        );

        // DrawLineEx(tr, br, thick, panelBorder);
    }

    if (borders.z != -1) {
        Color brdr = panelBorder;
        if (borders.z != 0) {
            brdr = GetColor((int)borders.z);
        }

        // DrawLineEx(tl, tr, thick, panelBorder);
        DrawRectangleRec(
            (Rectangle){innerRect.x, innerRect.y, innerRect.width, thick}, brdr
        );
    }

    if (borders.w != -1) {
        Color brdr = panelBorder;
        if (borders.w != 0) {
            brdr = GetColor((int)borders.w);
        }
        // DrawLineEx(bl, br, thick, panelBorder);
        DrawRectangleRec(
            (Rectangle){innerRect.x, innerRect.height + innerRect.y - thick,
                        innerRect.width, thick},
            brdr
        );
    }

    DrawRectangleRec((Rectangle){innerRect.x, innerRect.y, thick, thick}, sc);
    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick, innerRect.y, thick,
                    thick},
        sc
    );
    DrawRectangleRec(
        (Rectangle){innerRect.x, innerRect.y + innerRect.height - thick, thick,
                    thick},
        sc
    );
    DrawRectangleRec(
        (Rectangle){innerRect.x + innerRect.width - thick,
                    innerRect.y + innerRect.height - thick, thick, thick},
        sc
    );
}
