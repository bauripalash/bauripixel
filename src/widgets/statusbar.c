#include "../include/widgets/statusbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include <stdbool.h>
#include <stdlib.h>

#define SB_MARGIN_TB      5
#define SB_MARGIN_LR      5
#define STATUSBAR_PADDING 5

StatusBarState NewStatusBar() {
    StatusBarState sb = {0};

    sb.prop = NewWidgetProp();
    sb.prop.active = true;
    sb.anchor = (Vector2){0, 0};
    sb.canvas = NULL;
    sb.colorbar = NULL;
    sb.layerbar = NULL;

    return sb;
}

static void updateBounds(StatusBarState *state) {
    state->prop.bounds.x = SB_MARGIN_LR;
    state->prop.bounds.width = GetScreenWidth() - SB_MARGIN_LR * 2;
    state->prop.bounds.y =
        GetScreenHeight() - state->prop.bounds.height - SB_MARGIN_TB;
}

void SetStatusBarPosition(StatusBarState *state, float x, float height) {
    state->prop.bounds.height = height;
    updateBounds(state);
}
static void drawBounds(Rectangle bounds) { BpPanelBorder(bounds, 2); }

bool StatusBar(StatusBarState *state) {
    if (state->prop.active) {

        updateBounds(state);

        BpPanelBorder(state->prop.bounds, 2);
        Font font = GuiGetFont();
        float fontSize = font.baseSize;
        Color textClr = ColorVWhite;
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

        if (state->layerbar->draggingLayer &&
            state->layerbar->dragLayer != NULL) {

            int targetIndex = state->layerbar->dragTarget;
            LayerObj *targetLayer =
                GetLayerFromList(state->layerbar->list, targetIndex);
            if (targetLayer == NULL) {
                return false;
            }

            const char *targetName = targetLayer->name;
            if (targetName == NULL || state->layerbar->dragLayer == NULL) {
                return false;
            }
            LayerObj *selectLayer = state->layerbar->dragLayer;
            int selectIndex = selectLayer->index;
            const char *selectName = selectLayer->name;
            if (selectName == NULL) {
                return false;
            }
            if (selectIndex == targetIndex) {
                return false;
            }
            bool below = state->layerbar->putDragAtEnd;

            const char *text = TextFormat(
                "Moving layer '%s' (#%d) %s '%s' (#%d)", selectName,
                selectIndex, below ? "below" : "above", targetName, targetIndex
            );
            DrawTextEx(font, text, textPos, fontSize, 0, textClr);
        }

        // drawBounds(state->prop.bounds);
    }
    return false;
}
