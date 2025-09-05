#include "../include/windows/window.h"
#include <stdbool.h>

WinProp NewWinProp() {
    WinProp p = {0};
    p.active = false;
    p.b = (Rectangle){0};
    p.customPos = false;
    p.drag = false;
    p.resize = false;
    return p;
}
