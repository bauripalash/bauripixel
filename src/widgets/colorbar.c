#include "../include/widgets/colorbar.h"
#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/stb/stb_ds.h"
#include <stdbool.h>

ColorBarState NewColorBar() {
    ColorBarState cb = {0};
    cb.prop = NewWidgetProp();

    cb.currentIndex = -1;
    cb.currentColor = NOCOLOR;
    cb.colors = NULL;
    cb.colorCount = 0;

    cb.anchor = (Vector2){0, 0};
    cb.bottomStop = (Vector2){0, 0};
    cb.scroll = (Vector2){0, 0};

    cb.boxSize = DEFAULT_BOX_SIZE;
    cb.boxSpacing = DEFAULT_BOX_SPACE;

    cb.content = (Rectangle){0, 0, DEFAULT_BOX_SIZE,
                             DEFAULT_BOX_SPACE + DEFAULT_BOX_SIZE};
    cb.view = (Rectangle){0};
    cb.widthDragging = false;
    cb.heightDragging = false;
    cb.scrollBarWidth = GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH);

    cb.prop.bounds.width = DEFAULT_BOX_SIZE + (DEFAULT_BOX_SPACE * 2) +
                           cb.scrollBarWidth + MARGIN_LEFT * 2;
    cb.prop.bounds.height =
        GetScreenHeight() - cb.anchor.y - cb.bottomStop.y -
        MARGIN_TOPBOTTOM * 2; // how to handle according to anchor.y

    return cb;
}

void SetColorBarAnchor(ColorBarState *cb, Vector2 anchor, Vector2 bottom) {
    if (anchor.x != -1) {
        cb->anchor.x = anchor.x;
    }
    if (anchor.y != -1) {
        cb->anchor.y = anchor.y;
    }
    if (bottom.x != -1) {
        cb->bottomStop.x = bottom.x;
    }

    if (bottom.y != -1) {
        cb->bottomStop.y = bottom.y;
    }

    cb->prop.bounds.height =
        GetScreenHeight() - cb->anchor.y - cb->bottomStop.y -
        MARGIN_TOPBOTTOM * 2; // how to handle according to anchor.y
}

int AddToColorBar(ColorBarState *cb, Color color) {
    arrpush(cb->colors, color);
    cb->colorCount = arrlen(cb->colors);

    return cb->colorCount - 1;
}

void ClearColorBar(ColorBarState *cb) {
    if (cb->colors != NULL) {
        arrfree(cb->colors);
    }
}

bool ColorBar(ColorBarState *state) {
    if (state->prop.active) {

        Vector2 mpos = GetMousePosition();
        int boxSize = state->boxSize;
        int boxSpacing = state->boxSpacing;

        int minScrollSize =
            boxSpacing + boxSize + boxSpacing + state->scrollBarWidth;
        int minSize = minScrollSize + MARGIN_LEFT * 2;
        state->prop.bounds.x = state->anchor.x + MARGIN_LEFT;
        state->prop.bounds.y = state->anchor.y + MARGIN_TOPBOTTOM;
        // state->prop.bounds.height =
        //     GetScreenHeight() - state->anchor.y - state->bottomStop.y -
        //     MARGIN_TOPBOTTOM * 2; // how to handle according to anchor.y

        Rectangle bounds = state->prop.bounds;

        Rectangle hResizeHandle = {
            bounds.x + bounds.width - HANDLE_THICKNESS * 0.5f, bounds.y,
            HANDLE_THICKNESS, bounds.height
        };
        Rectangle vResizeHandle = {
            bounds.x,
            bounds.y + bounds.height - HANDLE_THICKNESS * 0.5f,
            bounds.width,
            HANDLE_THICKNESS,
        };

        // Width Slider
        bool hHandleHover = false;
        bool vHandleHover = false;
        if (CheckCollisionPointRec(mpos, hResizeHandle)) {
            hHandleHover = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                state->widthDragging = true;
            }
        } else {
            hHandleHover = false;
        }

        if (CheckCollisionPointRec(mpos, vResizeHandle)) {
            vHandleHover = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                state->heightDragging = true;
            }
        } else {
            vHandleHover = false;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            state->widthDragging = false;
            state->heightDragging = false;
        }

        if (state->widthDragging) {
            state->prop.bounds.width = mpos.x - state->prop.bounds.x;

            if (state->prop.bounds.width < minSize) {
                state->prop.bounds.width = minSize;
            }
        }

        int minHeight =
            boxSize + boxSpacing * 2 + state->scrollBarWidth + PANEL_PADDING;

        if (state->heightDragging) {
            state->prop.bounds.height = mpos.y - state->prop.bounds.y;
            if (state->prop.bounds.height < minHeight) {
                state->prop.bounds.height = minHeight;
            }
        }

        bounds.width = state->prop.bounds.width;
        bounds.height = state->prop.bounds.height;

        DrawRectangleRec(bounds, LIGHTGRAY);
        DrawRectangleLinesEx(bounds, 2, BLACK);

        if (hHandleHover) {
            Color clr = state->widthDragging ? BLUE : MAGENTA;
            DrawRectangleRec(hResizeHandle, clr);
        }

        if (vHandleHover) {
            Color clr = state->heightDragging ? BLUE : MAGENTA;
            DrawRectangleRec(vResizeHandle, clr);
        }

        Rectangle scrollBound = {
            bounds.x + PANEL_PADDING,
            bounds.y + PANEL_PADDING,
            bounds.width - (PANEL_PADDING * 2),
            bounds.height - (PANEL_PADDING * 2),
        };

        // How many color boxes can be in a row
        int cBoxCols = scrollBound.width / (boxSize + boxSpacing * 2);

        int colorCount = arrlen(state->colors);
        if (cBoxCols < 1) {
            cBoxCols = 1;
        }

        // How many rows there will be
        int cBoxRows = (colorCount + cBoxCols + 1) / cBoxCols;

        // total amount will the color boxes will take
        int totalCBoxWidth = cBoxCols * boxSize + (cBoxCols - 1) * boxSpacing;

        int boxesMargin = (scrollBound.width - totalCBoxWidth) / 2;
        if (boxesMargin < boxSpacing) {
            boxesMargin = boxSpacing;
        }

        state->content.width = cBoxCols * (boxSize + boxSpacing);
        state->content.height =
            PANEL_PADDING + cBoxRows * (boxSize + boxSpacing);

        GuiScrollPanel(
            scrollBound, NULL, state->content, &state->scroll, &state->view
        );

        BeginScissorMode(
            scrollBound.x, scrollBound.y, scrollBound.width, scrollBound.height
        );

        for (int i = 0; i < colorCount; i++) {
            int row = i / cBoxCols;
            int col = i % cBoxCols;

            float x =
                scrollBound.x + boxesMargin + col * (boxSize + boxSpacing);
            float y = scrollBound.y + MARGIN_TOPBOTTOM +
                      row * (boxSize + boxSpacing) + state->scroll.y;
            Rectangle rect = {x, y, boxSize, boxSize};
            DrawRectangleRec(rect, state->colors[i]);
            DrawRectangleLinesEx(rect, 2, BLACK);

            if (state->currentIndex == i) {
                DrawRectangleLinesEx(rect, 3, BLACK);
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                CheckCollisionPointRec(mpos, rect)) {
                if (state->currentIndex == i) {
                    state->currentIndex = -1;
                    state->currentColor = NOCOLOR;
                } else {
                    state->currentIndex = i;
                    state->currentColor = state->colors[i];
                }
            }
        }

        EndScissorMode();
    }
    return false;
}
