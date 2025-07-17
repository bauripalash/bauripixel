#include "../external/raygui.h"
#include "../external/raylib.h"
#include "../external/raymath.h"
#include "../include/colors.h"
#include "../include/components.h"
#include "../include/options.h"
#include <stdbool.h>
#include <stdlib.h>

#define SLIDER_PADDING_TB 5.0f
#define SLIDER_PADDING_LR 5.0f

bool BpSlider(Rectangle bounds, float *value, float min, float max) {
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

    float oldValue = *value;

    float scale = (*value - min) / (max - min);
    float thumbW = scale * rect.width;

    Rectangle thumbRect = {rect.x, rect.y, thumbW, rect.height};

    Vector2 mpos = GetMousePosition();
    if (CheckCollisionPointRec(mpos, bounds)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            float disp = (mpos.x - rect.x) / rect.width;
            disp = Clamp(disp, 0.0f, 1.0f);
            *value = min + disp * (max - min);
        }
    }

    *value = Clamp(*value, min, max);

    float fontW = GetFontDefault().recs->height;
    float fontH = GetFontDefault().baseSize;
    float textWidth = TextLength(TextFormat("%0.0fpx", *value));
    float textX = ((rect.width + rect.x) / 2.0f) - (textWidth / 2.0f);
    float textY = rect.y;

    Color textColor = ((thumbRect.x + thumbRect.width) >= textX + textWidth)
                          ? textPost
                          : textPre;

    DrawRectangleRounded(bounds, 0.1, 10, sliderBg);
    DrawRectangleRounded(thumbRect, 0.3, 0, sliderFg);
    DrawRectangleRoundedLinesEx(bounds, 0.1, 0, 2, sliderBorder);
    DrawText(TextFormat("%0.0fpx", *value), textX, textY, fontH, textColor);

    return oldValue != *value;
}
bool BpSliderInt(Rectangle bounds, int *value, int min, int max) {
    return false;
}

// BUG: There is +1 delay from slider to inputbox

bool BpInputSlider(
    Rectangle bounds, float *value, float min, float max, char *strValue,
    bool *clicked
) {
    Color inputBg = GetColor(OptThemeGet(T_ISLIDER_BG));
    int textClrInt = OptThemeGet(T_ISLIDER_FG);
    Color inputFg = GetColor(textClrInt);
    float oldValue = *value;
    Vector2 mpos = GetMousePosition();

    TextCopy(strValue, TextFormat("%d", (int)*value));

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

    if (*clicked) {
        BpSlider(sliderRect, value, min, max);
        TextCopy(strValue, TextFormat("%d", (int)*value));
    }

    GuiValueBoxFloat(boxRect, NULL, strValue, value, *clicked);
    // DrawRectangleLinesEx(boxRect, 2, inputBg);
    *value = (float)TextToInteger(strValue);
    *value = Clamp(*value, min, max);

    float fontH = GetFontDefault().baseSize;

    DrawText(
        "px", boxRect.x + boxRect.width, boxRect.y + fontH / 2.0f,
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
