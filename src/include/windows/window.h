#ifndef WINDOW_H
#define WINDOW_H

#include "../../external/raylib.h"
#include <stdbool.h>

typedef struct WinProp {
    // Is window visible
    bool active;
    // Window Bounds
    Rectangle b;
} WinProp;

WinProp NewWinProp();

#endif
