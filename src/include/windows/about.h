#ifndef WIN_ABOUT_H
#define WIN_ABOUT_H

#include "window.h"

// About Window state
typedef struct WAboutState {
	// Base window prop
    WinProp p;
} WAboutState;

// Create about window state
WAboutState NewWAbout();
// Draw about window
// Return status (close, ok clicked, cancel clicked...)
WinStatus WAbout(WAboutState *state, double dt);
// Free about window status
void FreeWAbout(WAboutState *state);

#endif
