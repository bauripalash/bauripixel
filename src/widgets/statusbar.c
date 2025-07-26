#include "../include/widgets/statusbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include <stdbool.h>
#include <stdio.h>

#define STATUSBAR_MARGIN  2
#define STATUSBAR_PADDING 5

StatusBarState NewStatusBar() {
    StatusBarState sb = {0};

    sb.prop = NewWidgetProp();
    sb.prop.active = true;
    sb.anchor = (Vector2){0, 0};
    sb.canvas = NULL;
    sb.colorbar = NULL;

    return sb;
}
void SetStatusBarPosition(StatusBarState *state, float x, float height) {
    state->prop.bounds.height = height;
}

static void updateBounds(StatusBarState *state) {
    state->prop.bounds.x = STATUSBAR_MARGIN;
    state->prop.bounds.width = GetScreenWidth() - STATUSBAR_MARGIN * 2;
    state->prop.bounds.y =
        GetScreenHeight() - state->prop.bounds.height - STATUSBAR_MARGIN;
}

static void drawBounds(Rectangle bounds) {
    DrawRectangleRounded(bounds, 0.125, 0, ColorFDGrayLighter);
    DrawRectangleRoundedLinesEx(bounds, 0.125, 0, 2, ColorBlack);

    DrawRectangleRoundedLinesEx(
        (Rectangle){
            bounds.x + 2,
            bounds.y + 2,
            bounds.width - 4,
            bounds.height - 4,
        },
        0.125, 0, 2, ColorGrayLightest
    );
}

bool StatusBar(StatusBarState *state) {
    if (state->prop.active) {

        updateBounds(state);
        Font font = GuiGetFont();
        float fontSize = font.baseSize;
        Color textClr = ColorGrayLightest;
        CanvasState *canvas = state->canvas;
        Rectangle rect = {
            state->prop.bounds.x + STATUSBAR_PADDING, state->prop.bounds.y, 32,
            state->prop.bounds.height
        };

        Vector2 textPos = {rect.x, rect.y + ((rect.height - fontSize) / 2.0f)};

        // if ColorHover : TDO

        if (state->canvas->hoverCanvas) {
            GuiDrawIcon(ICON_MODE_2D, textPos.x, textPos.y, 1, textClr);
            textPos.x += fontSize;
            const char *hoverText =
                TextFormat("[%d %d]", canvas->hoverX + 1, canvas->hoverY + 1);
            DrawTextEx(font, hoverText, textPos, fontSize, 0, textClr);
            textPos.x += 5 + MeasureTextEx(font, hoverText, fontSize, 0).x;
        }

        if (state->colorbar->hoverColorList) {

            GuiDrawIcon(ICON_BRUSH_PAINTER, textPos.x, textPos.y, 1, textClr);
            textPos.x += 5 + fontSize;
            Rectangle cPreview = {textPos.x, textPos.y, rect.height, fontSize};
            DrawRectangleRounded(cPreview, 0.2, 0, state->colorbar->hoverColor);
            DrawRectangleRoundedLinesEx(cPreview, 0.2, 0, 2, textClr);

            textPos.x += cPreview.width + 5;
            Color hc = state->colorbar->hoverColor;
            const char *colorText = TextFormat(
                "#%d | (RGBA %d, %d, %d, %d) | (Hex #%x)",
                state->colorbar->hoverIndex, hc.r, hc.g, hc.b, hc.a,
                ColorToInt(hc)
            );

            DrawTextEx(font, colorText, textPos, fontSize, 0, textClr);
            textPos.x += 5 + MeasureTextEx(font, colorText, fontSize, 0).x;
        }

        drawBounds(state->prop.bounds);
    }
    return false;
}
