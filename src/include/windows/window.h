#ifndef WINDOW_H
#define WINDOW_H

#include "../../external/raylib.h"
#include <stdbool.h>

// Window Status
typedef enum WinStatus {
	// Do nothing
    WIN_NONE = 0,
	// Ok/Save clicked
    WIN_OK = 1,
	// Cancel clicked
    WIN_CANCEL = 2,
	// Close window button clicked
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

// Create new window prop
WinProp NewWinProp();

#endif
