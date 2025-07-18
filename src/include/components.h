#ifndef COMPONENETS_H
#define COMPONENETS_H

#include "../external/raylib.h"
#include <stdbool.h>

bool BpInputSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit,
    bool *clicked
);
bool BpSliderInt(
    Rectangle bounds, int *value, int min, int max, const char *unit
);

#endif
