#ifndef WINDOW_H
#define WINDOW_H

#include "../../external/raylib.h"
#include <stdbool.h>

typedef enum WinStatus {
    WIN_NONE = 0,
    WIN_OK = 1,
    WIN_CANCEL = 2,
    WIN_CLOSE = 3
} WinStatus;

typedef struct WinProp {
    // Is window visible
    bool active;
    // Window Bounds
    Rectangle b;
    // User dragged the window
    bool customPos;
    // Is Window being Dragged to change position
    bool drag;
    // Is Window being resized
    bool resize;
} WinProp;

WinProp NewWinProp();

#endif
