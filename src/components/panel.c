#include "../components.h"
#include "../external/raylib/raylib.h"
#include "../options.h"
#include "raylib.h"

#define TLDot() (DrawRectangle(bounds.x, bounds.y, thickness, thickness, brdr))
#define TRDot()                                                                \
    (DrawRectangle(                                                            \
        bounds.x + bounds.width - thickness, bounds.y, thickness, thickness,   \
        brdr                                                                   \
    ))
#define BLDot()                                                                \
    (DrawRectangle(                                                            \
        bounds.x, bounds.y + bounds.height - thickness, thickness, thickness,  \
        brdr                                                                   \
    ))
#define BRDot()                                                                \
    (DrawRectangle(                                                            \
        bounds.x + bounds.width - thickness,                                   \
        bounds.y + bounds.height - thickness, thickness, thickness, brdr       \
    ))

void BpSimplePanel(Rectangle bounds, int thickness, BpSide sides) {
    Color bg = GetColor(OptThemeGet(T_PANEL_BG));
    Color brdr = GetColor(OptThemeGet(T_PANEL_BORDER));
    DrawRectangleRec(
        (Rectangle){
            bounds.x - thickness, bounds.y, bounds.width + thickness * 2,
            bounds.height
        },
        brdr
    );
    DrawRectangle(
        bounds.x, bounds.y - thickness, bounds.width,
        bounds.height + thickness * 2, brdr
    );

    // Left + Right Background
    DrawRectangle(
        bounds.x, bounds.y + thickness, bounds.width,
        bounds.height - thickness * 2, bg
    );
    // Top + Bottom Background
    DrawRectangle(
        bounds.x + thickness, bounds.y, bounds.width - thickness * 2,
        bounds.height, bg
    );

    if (!IsAllSide(sides)) {
        if (sides.top == -1) {
            DrawRectangle(
                bounds.x, bounds.y - thickness, bounds.width, bounds.height, bg
            );

            BLDot();
            BRDot();
        }

        if (sides.bottom == -1) {
            DrawRectangle(
                bounds.x, bounds.y, bounds.width, bounds.height + thickness, bg
            );
            TLDot();
            TRDot();
        }

        if (sides.left == -1) {
            DrawRectangle(
                bounds.x - thickness, bounds.y, bounds.width + thickness,
                bounds.height, bg
            );
            TRDot();
            BRDot();
        }

        if (sides.right == -1) {
            DrawRectangle(
                bounds.x, bounds.y, bounds.width + thickness, bounds.height, bg
            );
            TLDot();
            BLDot();
        }
    }
}

void BpPanelBorder(Rectangle bounds, int thickness) {
    BpSimplePanel(bounds, thickness, MakeAllSide());
}
void BpPanelOnlyBorder(Rectangle bounds, int thickness) {}
void BpPanelNoBorder(Rectangle bounds) {}
