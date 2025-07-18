#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/options.h"
#include "../include/utils.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define SLIDER_PADDING_TB 5.0f
#define SLIDER_PADDING_LR 5.0f

bool BpSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit
) {
    Color sliderBg = GetColor(OptThemeGet(T_SLIDER_BG));
    Color sliderBorder = GetColor(OptThemeGet(T_SLIDER_BORDER));
    Color sliderFg = GetColor(OptThemeGet(T_SLIDER_FG));
    Color textPre = GetColor(OptThemeGet(T_SLIDER_TEXT_PRE));
    Color textPost = GetColor(OptThemeGet(T_SLIDER_TEXT_POST));

    Rectangle rect = {
        bounds.x + SLIDER_PADDING_LR,
        bounds.y + SLIDER_PADDING_TB,
        bounds.width - SLIDER_PADDING_LR * 2.0f,
        bounds.height - SLIDER_PADDING_TB * 2.0f,
    };

    int oldValue = *value;
    float thumbW = rect.width * (float)(*value - min) / (max - min);

    Rectangle thumbRect = {rect.x, rect.y, thumbW, rect.height};

    Vector2 mpos = GetMousePosition();

    if (CheckCollisionPointRec(mpos, bounds)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            float percent = (mpos.x - rect.x) / rect.width;
            *value = min + (int)ceilf(percent * (max - min));
        }

        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            *value += wheel;
        }
    }

    *value = ClampInt(*value, min, max);

    Font font = GetFontDefault();
    float fontW = font.recs->width;
    float fontH = font.baseSize;
    float textWidth = TextLength(TextFormat("%d%s", *value, unit));
    float textX = ((rect.width + rect.x) / 2.0f) - (textWidth / 2.0f);
    float textY = rect.y;
    Color textColor = ((thumbRect.x + thumbRect.width) >= textX + textWidth)
                          ? textPost
                          : textPre;

    DrawRectangleRounded(bounds, 0.1, 0, sliderBg);
    DrawRectangleRounded(thumbRect, 0.1, 0, sliderFg);

    DrawText(TextFormat("%d%s", *value, unit), textX, textY, fontH, textColor);

    return oldValue != *value;
}

bool BpInputSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit,
    bool *clicked
) {
    Color inputBg = GetColor(OptThemeGet(T_ISLIDER_BG));
    int textClrInt = OptThemeGet(T_ISLIDER_FG);
    Color inputFg = GetColor(textClrInt);
    float oldValue = *value;
    Vector2 mpos = GetMousePosition();

    int ogBoxBorderW = GuiGetStyle(VALUEBOX, BORDER_WIDTH);
    int ogBoxBorderN = GuiGetStyle(VALUEBOX, BORDER_COLOR_PRESSED);
    int ogBoxBaseN = GuiGetStyle(VALUEBOX, BASE_COLOR_NORMAL);
    int ogBoxBaseF = GuiGetStyle(VALUEBOX, BASE_COLOR_FOCUSED);
    int ogBoxBaseP = GuiGetStyle(VALUEBOX, BASE_COLOR_PRESSED);
    int ogBoxTextN = GuiGetStyle(VALUEBOX, TEXT_COLOR_NORMAL);
    int ogBoxTextF = GuiGetStyle(VALUEBOX, TEXT_COLOR_FOCUSED);
    int ogBoxTextP = GuiGetStyle(VALUEBOX, TEXT_COLOR_PRESSED);

    GuiSetStyle(VALUEBOX, BORDER_WIDTH, 0);
    GuiSetStyle(VALUEBOX, BORDER_COLOR_PRESSED, textClrInt);

    GuiSetStyle(VALUEBOX, BASE_COLOR_NORMAL, 0);
    GuiSetStyle(VALUEBOX, BASE_COLOR_FOCUSED, 0);
    GuiSetStyle(VALUEBOX, BASE_COLOR_PRESSED, 0);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_NORMAL, textClrInt);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_FOCUSED, textClrInt);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_PRESSED, textClrInt);

    Rectangle sliderRect = {bounds.x, bounds.y + bounds.height + 5, 150, 25};

    Rectangle boxRect = {
        bounds.x + 5,
        bounds.y + 5,
        bounds.width - 20,
        bounds.height - 10,
    };

    bool atBounds = CheckCollisionPointRec(mpos, bounds);
    bool atSlider = CheckCollisionPointRec(mpos, sliderRect);

    if (atBounds && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *clicked = true;
    }

    if (!atBounds && !atSlider && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *clicked = false;
    }

    DrawRectangleRounded(bounds, 0.2, 0, Fade(inputBg, 0.8));
    DrawRectangleRoundedLinesEx(bounds, 0.2, 0, 0, ColorGrayLightest);

    GuiValueBox(boxRect, NULL, value, min, max, *clicked);

    if (*clicked) {
        if (IsKeyPressed(KEY_UP)) {
            *value += 1;
        }

        if (IsKeyPressed(KEY_DOWN)) {
            *value -= 1;
        }
    }

    *value = ClampInt(*value, min, max);

    if (*clicked) {
        BpSliderInt(sliderRect, value, min, max, unit);
    }

    float fontH = GetFontDefault().baseSize;

    DrawText(
        unit, boxRect.x + boxRect.width, boxRect.y + fontH / 2.0f,
        GetFontDefault().baseSize, inputFg
    );

    GuiSetStyle(VALUEBOX, BORDER_WIDTH, ogBoxBorderW);

    GuiSetStyle(VALUEBOX, BORDER_COLOR_PRESSED, ogBoxBorderN);
    GuiSetStyle(VALUEBOX, BASE_COLOR_NORMAL, ogBoxBaseN);
    GuiSetStyle(VALUEBOX, BASE_COLOR_FOCUSED, ogBoxBaseF);
    GuiSetStyle(VALUEBOX, BASE_COLOR_PRESSED, ogBoxBaseP);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_NORMAL, ogBoxTextN);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_FOCUSED, ogBoxTextF);
    GuiSetStyle(VALUEBOX, TEXT_COLOR_PRESSED, ogBoxTextP);

    return oldValue != *value;
}
