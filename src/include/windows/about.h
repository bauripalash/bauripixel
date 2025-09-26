#ifndef WIN_ABOUT_H
#define WIN_ABOUT_H

#include "window.h"

typedef struct WAboutState {
    WinProp p;
} WAboutState;

WAboutState NewWAbout();
WinStatus WAbout(WAboutState *state, double dt);
void FreeWAbout(WAboutState *state);

#endif
