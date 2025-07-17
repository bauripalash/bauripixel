#ifndef COMPONENETS_H
#define COMPONENETS_H

#include "../external/raylib.h"
#include <stdbool.h>

bool BpInputSlider(
    Rectangle bounds, float *value, float min, float max, char *strValue,
    bool *clicked
);
bool BpSlider(Rectangle bounds, float *value, float min, float max);
bool BpSliderInt(Rectangle bounds, int *value, int min, int max);

#endif
