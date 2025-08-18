#include "../include/windows/window.h"
#include <stdbool.h>

WinProp NewWinProp() {
    WinProp p = {0};
    p.active = false;
    p.b = (Rectangle){0};
    return p;
}
